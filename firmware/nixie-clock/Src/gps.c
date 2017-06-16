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
	return;
}
