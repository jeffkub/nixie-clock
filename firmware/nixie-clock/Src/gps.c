/*******************************************************************************
MIT License

Copyright (c) 2017 Jeff Kubascik

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

/* Includes *******************************************************************/
#include "gps.h"

#include "globals.h"
#include "stm32f3xx_hal.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "debug.h"
#include "rtc.h"
#include "uart3.h"


/* Private definitions ********************************************************/
typedef enum
{
    GPRMC_MessageID = 0,
    GPRMC_UTCTime,
    GPRMC_Status,
    GPRMC_Latitude,
    GPRMC_NSIndicator,
    GPRMC_Longitude,
    GPRMC_EWIndicator,
    GPRMC_SpeedOverGround,
    GPRMC_CourseOverGround,
    GPRMC_Date,
    GPRMC_MagneticVariation,
    GPRMC_Mode,
    GPRMC_Checksum,

    GPRMC_Count
} GPRMC_Fields;

typedef struct
{
    struct tm dateTime;
    int       subsec;
} timestamp_t;

#define SUBSEC_SAMPLE_COUNT 10


/* Private variables **********************************************************/
static QueueHandle_t timestampQueue;

static int errorSum = 0;
static int errorCount = 0;


/* Private function prototypes ************************************************/
static void gpsEnable(bool state);
static void syncRtcToGps(timestamp_t * ppsTimestamp, struct tm * gpsDateTime);
static void handleRMC(char ** dataItems, size_t dataItemsCount);
static ssize_t splitSentence(char * str, char ** tokvec, size_t tokvecLen);
static void gpsTask(void * argument);


/* Private function definitions ***********************************************/
static void gpsEnable(bool state)
{
    if(state)
    {
        HAL_GPIO_WritePin(GPS_RESET_GPIO_Port, GPS_RESET_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(GPS_RESET_GPIO_Port, GPS_RESET_Pin, GPIO_PIN_RESET);
    }
}

static void syncRtcToGps(timestamp_t * ppsTimestamp, struct tm * gpsDateTime)
{
    time_t ppsTime;
    time_t gpsTime;
    int    error;

    ppsTime = mktime(&ppsTimestamp->dateTime);
    gpsTime = mktime(gpsDateTime);

    /* Adjust local time to match GPS time */
    if(ppsTime == gpsTime)
    {
        /* RTC clock is ahead of the GPS clock */
        errorSum += ppsTimestamp->subsec;
        errorCount++;
    }
    else if(ppsTime == (gpsTime - 1))
    {
        /* RTC clock is behind the GPS clock */
        errorSum -= (SUBSEC_PER_SEC - ppsTimestamp->subsec);
        errorCount++;
    }
    else
    {
        /* RTC clock is out of sync with the GPS clock */
        rtc_setTime(gpsDateTime);

        /* Reset error counters */
        errorSum = 0;
        errorCount = 0;

        debug_printf("Set the RTC clock: ppsTime=%u subsec=%d gpsTime=%u\n",
            (unsigned)ppsTime, ppsTimestamp->subsec, (unsigned)gpsTime);
    }

    if(errorCount >= SUBSEC_SAMPLE_COUNT)
    {
        error = errorSum / SUBSEC_SAMPLE_COUNT;

        rtc_adjust(-error);

        debug_printf("Adjusted RTC clock: error=%d\n", error);

        errorSum = 0;
        errorCount = 0;
    }

    return;
}

static void handleRMC(char ** dataItems, size_t dataItemsCount)
{
    struct tm   gpsDateTime = {0};
    timestamp_t ppsTimestamp;

    /* Parse time from GPS sentence */
    if(dataItemsCount < GPRMC_Count)
    {
        return;
    }

    if(strcmp(dataItems[GPRMC_Status], "A") != 0)
    {
        /* GPS not locked */
        return;
    }

    if(sscanf(dataItems[GPRMC_UTCTime], "%02d%02d%02d",
        &gpsDateTime.tm_hour, &gpsDateTime.tm_min, &gpsDateTime.tm_sec) != 3)
    {
        return;
    }

    if(sscanf(dataItems[GPRMC_Date], "%02d%02d%02d",
        &gpsDateTime.tm_mday, &gpsDateTime.tm_mon, &gpsDateTime.tm_year) != 3)
    {
        return;
    }

    gpsDateTime.tm_mon  -= 1;
    gpsDateTime.tm_year += 100;

    /* Get RTC timestamp when the PPS signal was asserted */
    if(xQueueReceive(timestampQueue, &ppsTimestamp, 0) == pdFALSE)
    {
        /* No RTC timestamp was found */
        return;
    }

    syncRtcToGps(&ppsTimestamp, &gpsDateTime);

    return;
}

static ssize_t splitSentence(char * str, char ** tokvec, size_t tokvecLen)
{
    static const char* const delim = ",*";
    size_t count = 0;

    tokvec[count] = strsep(&str, delim);

    while(tokvec[count] != NULL)
    {
        count++;
        if(count == tokvecLen)
        {
            /* Ran out of space in tokvec */
            return -1;
        }

        tokvec[count] = strsep(&str, delim);
    }

    return count;
}

static void gpsTask(void * argument)
{
    char    sentence[128];
    char *  dataItems[32];
    ssize_t dataItemsCount;

    for(;;)
    {
        /* Read a sentence from the GPS module */
        if(uart3_gets(sentence, sizeof(sentence)) == NULL)
        {
            return;
        }

        /* Split the sentence */
        dataItemsCount = splitSentence(sentence, dataItems, sizeof(dataItems));
        if(dataItemsCount < 1)
        {
            continue;
        }

        /* Determine sentence type */
        if(strcmp(dataItems[0], "$GPRMC") == 0)
        {
            /* Recommended Minimum Navigation Information */
            handleRMC(dataItems, dataItemsCount);
        }
    }

    return;
}


/* Public function definitions ************************************************/
void EXTI15_10_IRQHandler(void)
{
    BaseType_t taskWoken = pdFALSE;
    timestamp_t timestamp;

    if(READ_BIT(EXTI->PR, EXTI_PR_PR13))
    {
        /* Acknowledge interrupt */
        SET_BIT(EXTI->PR, EXTI_PR_PR13);

        rtc_getTimeFromISR(&timestamp.dateTime, &timestamp.subsec);
        xQueueOverwriteFromISR(timestampQueue, &timestamp, &taskWoken);
    }

    portYIELD_FROM_ISR(taskWoken);

    return;
}

void gps_init(void)
{
    timestampQueue = xQueueCreate(1, sizeof(timestamp_t));
    debug_assert(timestampQueue);

    xTaskCreate(
        gpsTask,
        "gps",
        512,
        NULL,
        GPS_TASK_PRIORITY,
        NULL);

    /* PPS pin interrupt init */
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, EXTI15_10_IRQ_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    gpsEnable(true);

    return;
}
