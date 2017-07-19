#include "rtc.h"

#include "FreeRTOS.h"
#include "portmacro.h"
#include "semphr.h"

#include "stm32f3xx_hal.h"

static SemaphoreHandle_t wakeupSem;

void RTC_WKUP_IRQHandler(void)
{
    BaseType_t taskWoken = pdFALSE;

    CLEAR_BIT(RTC->ISR, RTC_ISR_WUTF);

    //xSemaphoreGiveFromISR(wakeupSem, &taskWoken);

    //portYIELD_FROM_ISR(taskWoken);

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
    WRITE_REG(RTC->WPR, 0xCA);
    WRITE_REG(RTC->WPR, 0x53);

    /* Check if RTC calendar needs to be initialized */
    if(!READ_BIT(RTC->ISR, RTC_ISR_INITS))
    {
        /* Enter initialization mode */
        SET_BIT(RTC->ISR, RTC_ISR_INIT);
        while(!READ_BIT(RTC->ISR, RTC_ISR_INITF));

        /* Program the prescalar values */
        WRITE_REG(RTC->PRER, 0x007F00FF);

        /* Configure for 24h time */
        CLEAR_BIT(RTC->CR, RTC_CR_FMT);

        /* Load default date and time regs */
        WRITE_REG(RTC->TR, 0x00000000);
        WRITE_REG(RTC->DR, 0x00172101);

        /* Exit initialization mode */
        CLEAR_BIT(RTC->ISR, RTC_ISR_INIT);
    }

    /* Disable the wakeup timer */
    CLEAR_BIT(RTC->CR, RTC_CR_WUTE);
    while(!READ_BIT(RTC->ISR, RTC_ISR_WUTWF));

    /* Clear wakeup flag */
    CLEAR_BIT(RTC->ISR, RTC_ISR_WUTF);

    /* Program value into the wakeup timer */
    WRITE_REG(RTC->WUTR, 0);

    /* Set the wakeup timer clock source */
    MODIFY_REG(RTC->CR, RTC_CR_WUCKSEL, 0x4 << RTC_CR_WUCKSEL_Pos);

    /* RTC WakeUpTimer Interrupt Configuration: EXTI configuration */
    __HAL_RTC_WAKEUPTIMER_EXTI_ENABLE_IT();

    __HAL_RTC_WAKEUPTIMER_EXTI_ENABLE_RISING_EDGE();

    /* Enable wakeup timer interrupt */
    SET_BIT(RTC->CR, RTC_CR_WUTIE);

    /* Enable the wakeup timer */
    SET_BIT(RTC->CR, RTC_CR_WUTE);

    /* Enable write protection */
    WRITE_REG(RTC->WPR, 0xFF);

    return;
}

void rtc_wait(void)
{
    xSemaphoreTake(wakeupSem, portMAX_DELAY);

    return;
}
