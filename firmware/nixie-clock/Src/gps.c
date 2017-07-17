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
static void handleRMC(const char * payload);
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

static void handleRMC(const char * payload)
{
    if(sscanf(payload, "%02d%02d%02d", &gpsTime.tm_hour, &gpsTime.tm_min, &gpsTime.tm_sec) != 3)
    {
        return;
    }

    return;
}

static void gpsTask(void const * argument)
{
    char   msg[128];
    char * saveptr;
    char * msgId;
    char * payload;

    for(;;)
    {
        /* Read a message from the GPS module */
        if(uart3_gets(msg, sizeof(msg)) == NULL)
        {
            return;
        }

        /* Tokenize the message */
        msgId = strtok_r(msg, ",", &saveptr);
        if(msgId == NULL)
        {
            continue;
        }

        payload = strtok_r(NULL, "\r\n", &saveptr);
        if(payload == NULL)
        {
            continue;
        }

        /* Determine message type */
        if(strcmp(msgId, "$GPRMC") == 0)
        {
            /* Recommended Minimum Navigation Information */
            handleRMC(payload);
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
