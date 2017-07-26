#include "gps.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "cmsis_os.h"

#include "stm32f3xx_hal.h"

#include "main.h"
#include "rtc.h"
#include "uart3.h"

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

static osThreadId gpsTaskHandle;

static void gpsEnable(bool state);
static void handleRMC(char ** dataItems, size_t dataItemsCount);
static ssize_t splitSentence(char * str, char ** tokvec, size_t tokvecLen);
static void gpsTask(void const * argument);

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
    struct tm ts;
    time_t    localTime;
    time_t    gpsTime;
    int32_t   subOffset;

    /* Get local RTC time */
    localTime = rtc_getTime();

    /* Get sub-second offset from 1PPS signal */
    subOffset = rtc_getTsOffset();
    if(subOffset < 0)
    {
        /* Time stamp wasn't received */
        return;
    }

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
        &ts.tm_hour, &ts.tm_min, &ts.tm_sec) != 3)
    {
        return;
    }

    if(sscanf(dataItems[GPRMC_Date], "%02d%02d%02d",
        &ts.tm_mday, &ts.tm_mon, &ts.tm_year) != 3)
    {
        return;
    }

    gpsTime = mktime(&ts);

    printf("localTime=%d, gpsTime=%d\n", (int)localTime, (int)gpsTime);

    /* Adjust local time to match GPS time */
    if(localTime == gpsTime)
    {
        /* Delay local time as needed */
        printf("Delaying RTC clock subOffset=%d\n", (int)subOffset);
        rtc_adjust(subOffset, false);
    }
    else if(localTime == (gpsTime - 1))
    {
        /* Advance local time as needed */
        printf("Advancing RTC clock subOffset=%d\n", (int)subOffset);
        rtc_adjust(subOffset, true);
    }
    else
    {
        /* Update RTC time */
        printf("Adjusting RTC clock\n");
        rtc_setTime(gpsTime);
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

void gps_init(void)
{
    osThreadDef(gpsTaskDef, gpsTask, osPriorityNormal, 0, 512);
    gpsTaskHandle = osThreadCreate(osThread(gpsTaskDef), NULL);

    gpsEnable(true);

    return;
}
