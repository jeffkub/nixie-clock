#include "rtc.h"

#include "FreeRTOS.h"
#include "portmacro.h"
#include "semphr.h"

#include "stm32f3xx_hal.h"

static SemaphoreHandle_t wakeupSem;

void RTC_WKUP_IRQHandler(void)
{
	BaseType_t taskWoken = pdFALSE;

	xSemaphoreGiveFromISR(wakeupSem, &taskWoken);

	portYIELD_FROM_ISR(taskWoken);

	return;
}

void rtc_init(void)
{
	wakeupSem = xSemaphoreCreateBinary();

	/* Peripheral clock enable */
	__HAL_RCC_RTC_ENABLE();

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);

	/* Disable write protection */
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

	return;
}

void rtc_wait(void)
{
	xSemaphoreTake(wakeupSem, portMAX_DELAY);

	return;
}
