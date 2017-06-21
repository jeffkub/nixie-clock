#include "gps.h"

#include <stdbool.h>

#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "semphr.h"

#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_uart.h"

#include "main.h"

extern UART_HandleTypeDef huart3;

static SemaphoreHandle_t devMutex;
static SemaphoreHandle_t doneSem;

static bool errorFlag;

static void gpsEnable(bool state);

static ssize_t uart_read(char * data, size_t len);

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

static ssize_t uart_read(char * data, size_t len)
{
	ssize_t retval = -1;

	errorFlag = false;

	/* Start RX */
	if(HAL_UART_Receive_IT(&huart3, (uint8_t *) data, len) != HAL_OK)
	{
		return -1;
	}

	/* Wait for RX to complete */
	xSemaphoreTake(doneSem, portMAX_DELAY);

	if(errorFlag == false)
	{
		retval = len;
	}

	return retval;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    BaseType_t taskWoken;

    errorFlag = false;

    xSemaphoreGiveFromISR(doneSem, &taskWoken);

    portYIELD_FROM_ISR(taskWoken);

    return;
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
    BaseType_t taskWoken;

    errorFlag = true;

    xSemaphoreGiveFromISR(doneSem, &taskWoken);

    portYIELD_FROM_ISR(taskWoken);

    return;
}

void gps_init(void)
{
    devMutex = xSemaphoreCreateMutex();
    doneSem = xSemaphoreCreateBinary();

    gpsEnable(true);

	return;
}

void gps_read(void)
{
	char buf;

	if(uart_read(&buf, 1) > 0)
	{
		printf("%c", buf);
	}

	return;
}
