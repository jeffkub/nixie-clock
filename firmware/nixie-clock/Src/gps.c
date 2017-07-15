#include "gps.h"

#include <ctype.h>
#include <stdbool.h>

#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "semphr.h"

#include "stm32f3xx_hal.h"

#include "main.h"
#include "uart3.h"

static void gpsEnable(bool state);

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

void gps_init(void)
{
    gpsEnable(true);

    return;
}

char line[128];

void gps_read(void)
{
    //char line[81];
    //int hour;
    //int min;
    //int sec;
    //int subsec;

    if(uart3_gets(line, sizeof(line)) == NULL)
    {
        return;
    }

    //if(sscanf(line, "$GPRMC,%02d%02d%02d.%03d", &hour, &min, &sec, &subsec) != 4)
    //{
    //    return;
    //}

    //printf("%d:%d:%d\n", hour, min, sec);

    return;
}
