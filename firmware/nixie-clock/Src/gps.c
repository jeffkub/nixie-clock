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
#include "cmsis_os.h"

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


/* Private variables **********************************************************/
static osThreadId gpsTaskHandle;

static struct tm ppsTimestamp;
static int       ppsTimestampSubsec;
static bool      ppsTimestampValid = false;


/* Private function prototypes ************************************************/
static void gpsEnable(bool state);
static void handleRMC(char ** dataItems, size_t dataItemsCount);
static ssize_t splitSentence(char * str, char ** tokvec, size_t tokvecLen);
static void gpsTask(void const * argument);


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

static void handleRMC(char ** dataItems, size_t dataItemsCount)
{
    time_t    rtcTime;
    struct tm rtcTimeStruct = {0};
    time_t    gpsTime;
    struct tm gpsTimeStruct = {0};
    int       subOffset;

    /* Get local RTC time */
    rtc_getTime(&rtcTimeStruct, &subOffset);
    rtcTime = mktime(&rtcTimeStruct);

    /* Parse time from GPS sentence */
    if(dataItemsCount < GPRMC_Count)
    {
        return;
    }

    /* Verify GPS data is valid (GPS lock) */
    if(strcmp(dataItems[GPRMC_Status], "A") != 0)
    {
        return;
    }

    if(sscanf(dataItems[GPRMC_UTCTime], "%02d%02d%02d",
        &gpsTimeStruct.tm_hour, &gpsTimeStruct.tm_min, &gpsTimeStruct.tm_sec) != 3)
    {
        return;
    }

    if(sscanf(dataItems[GPRMC_Date], "%02d%02d%02d",
        &gpsTimeStruct.tm_mday, &gpsTimeStruct.tm_mon, &gpsTimeStruct.tm_year) != 3)
    {
        return;
    }

    gpsTimeStruct.tm_mon  -= 1;
    gpsTimeStruct.tm_year += 100;

    gpsTime = mktime(&gpsTimeStruct);

    //debug_printf("rtcTime=%d, gpsTime=%d\n", (int)rtcTime, (int)gpsTime);

    /* Adjust local time to match GPS time */
    if(rtcTime == gpsTime)
    {
        /* RTC clock is ahead of the GPS clock */
        //rtc_adjust(subOffset, false);
    }
    else if(rtcTime == (gpsTime - 1))
    {
        /* RTC clock is behind the GPS clock */
        //rtc_adjust(subOffset, true);
    }
    else
    {
        /* RTC clock is out of sync with the GPS clock */
        rtc_setTime(&gpsTimeStruct);

        debug_printf("Adjusted RTC clock\n");
    }

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

static void gpsTask(void const * argument)
{
    char    sentence[128];
    char *  dataItems[32];
    ssize_t dataItemsCount;

    /* This task is going to use floating point operations */
    portTASK_USES_FLOATING_POINT();

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
    if(READ_BIT(EXTI->PR, EXTI_PR_PR13))
    {
        /* Acknowledge interrupt */
        SET_BIT(EXTI->PR, EXTI_PR_PR13);

        /* Get the RTC time */
        rtc_getTimeFromISR(&ppsTimestamp, &ppsTimestampSubsec);
        ppsTimestampValid = true;
    }

    return;
}

void gps_init(void)
{
    /* PPS pin interrupt init */
    HAL_NVIC_SetPriority(EXTI15_10_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

    osThreadDef(gpsTaskDef, gpsTask, osPriorityNormal, 0, 512);
    gpsTaskHandle = osThreadCreate(osThread(gpsTaskDef), NULL);

    gpsEnable(true);

    return;
}
