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

static ssize_t read_line(char * str, size_t len);

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

static ssize_t read_line(char * str, size_t len)
{
    size_t read = 0;

    while(true)
    {
        if(uart3_read(&str[read], 1) < 0)
        {
            /* Read error */
            continue;
        }

        if(read == 0 && isspace(str[read]) != 0)
        {
            /* Skip leading whitespace */
            continue;
        }
        else if(str[read] == '\r' || str[read] == '\n')
        {
            /* Reached end of line.  Null terminate string and exit loop */
            str[read] = '\0';
            read++;
            break;
        }

        read++;
        if(read == len)
        {
            /* Overflow, drop line */
            read = 0;
        }
    }

    return read;
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

    if(read_line(line, sizeof(line)) <= 0)
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
