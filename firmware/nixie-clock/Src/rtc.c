#include "rtc.h"

#include "FreeRTOS.h"
#include "portmacro.h"
#include "semphr.h"

#include "stm32f3xx_hal.h"

static SemaphoreHandle_t wakeupSem;

static uint8_t bcdToByte(uint8_t val);

static uint8_t bcdToByte(uint8_t val)
{
    uint32_t tmp = 0;

    tmp = ((uint8_t)(val & (uint8_t)0xF0U) >> (uint8_t)0x4U) * 10U;
    
    return (tmp + (val & (uint8_t)0x0FU));
}

void RTC_WKUP_IRQHandler(void)
{
    BaseType_t taskWoken = pdFALSE;

    if(READ_BIT(RTC->ISR, RTC_ISR_WUTF))
    {
        /* Clear interrupt flag */
        CLEAR_BIT(RTC->ISR, RTC_ISR_WUTF);
        WRITE_REG(EXTI->PR, RTC_EXTI_LINE_WAKEUPTIMER_EVENT);

        xSemaphoreGiveFromISR(wakeupSem, &taskWoken);
    }

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

    /* EXTI configuration for rising edge */
    SET_BIT(EXTI->IMR, RTC_EXTI_LINE_WAKEUPTIMER_EVENT);
    SET_BIT(EXTI->RTSR, RTC_EXTI_LINE_WAKEUPTIMER_EVENT);

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

time_t rtc_time(void)
{
    struct tm time;
    uint32_t  tr;
    uint32_t  dr;

    /* Read time and date registers */
    while(!READ_BIT(RTC->ISR, RTC_ISR_RSF));
    tr = READ_REG(RTC->TR);
    dr = READ_REG(RTC->DR);
    CLEAR_BIT(RTC->ISR, RTC_ISR_RSF);

    /* Process registers */
    time.tm_sec  = bcdToByte((tr & (RTC_TR_ST  | RTC_TR_SU )) >> RTC_TR_SU_Pos );
    time.tm_min  = bcdToByte((tr & (RTC_TR_MNT | RTC_TR_MNU)) >> RTC_TR_MNU_Pos);
    time.tm_hour = bcdToByte((tr & (RTC_TR_HT  | RTC_TR_HU )) >> RTC_TR_HU_Pos );

    time.tm_mday = bcdToByte((dr & (RTC_DR_DT  | RTC_DR_DU )) >> RTC_DR_DU_Pos );
    time.tm_mon  = bcdToByte((dr & (RTC_DR_MT  | RTC_DR_MU )) >> RTC_DR_MU_Pos ) - 1;
    time.tm_year = bcdToByte((dr & (RTC_DR_YT  | RTC_DR_YU )) >> RTC_DR_YU_Pos ) + 100;

    /* TODO: Verify no timezone offset */
    return mktime(&time);
}
