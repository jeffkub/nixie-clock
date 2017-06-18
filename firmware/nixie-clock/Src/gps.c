#include "gps.h"

#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "semphr.h"

#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_uart.h"

extern UART_HandleTypeDef huart3;

static SemaphoreHandle_t devMutex;
static SemaphoreHandle_t doneSem;

static ssize_t uart_read(char * data, size_t len);

static ssize_t uart_read(char * data, size_t len)
{
	/* Start RX */
	if(HAL_UART_Receive_IT(&huart3, (uint8_t *) data, len) != HAL_OK)
	{
		return -1;
	}

	/* Wait for RX to complete */
	xSemaphoreTake(doneSem, portMAX_DELAY);

	return len;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    BaseType_t taskWoken;

    xSemaphoreGiveFromISR(doneSem, &taskWoken);

    portYIELD_FROM_ISR(taskWoken);

    return;
}

void gps_init(void)
{
    devMutex = xSemaphoreCreateMutex();
    doneSem = xSemaphoreCreateBinary();

	return;
}

void gps_read(void)
{
	char buf;

	uart_read(&buf, 1);
	printf("%c", buf);

	return;
}
