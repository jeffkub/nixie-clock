#include "gps.h"

#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "cmsis_os.h"

#include "stm32f3xx_hal.h"

#include "main.h"
#include "uart3.h"

static osThreadId gpsTaskHandle;

static void gpsEnable(bool state);
static void handleRMC(char ** dataItems, size_t dataItemsCount);
static ssize_t tokenize(char * str, char ** tokvec, size_t tokvecLen);
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

struct tm gpsTime;

static void handleRMC(char ** dataItems, size_t dataItemsCount)
{
    if(sscanf(dataItems[1], "%02d%02d%02d", &gpsTime.tm_hour, &gpsTime.tm_min, &gpsTime.tm_sec) != 3)
    {
        return;
    }

    return;
}

static ssize_t tokenize(char * str, char ** tokvec, size_t tokvecLen)
{
    size_t count = 0;
    char * saveptr;

    tokvec[0] = strtok_r(str, ",", &saveptr);

    while(tokvec[count] != NULL)
    {
        count++;
        if(count == tokvecLen)
        {
            /* Ran out of space in tokvec */
            return -1;
        }

        tokvec[count] = strtok_r(NULL, ",", &saveptr);
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
        dataItemsCount = tokenize(sentence, dataItems, sizeof(dataItems));
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
