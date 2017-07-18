#include "rtc.h"

#include "stm32f3xx_hal.h"

void rtc_init(void)
{
	/* Peripheral clock enable */
	__HAL_RCC_RTC_ENABLE();

	/* Disable write protection */
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

	
}
