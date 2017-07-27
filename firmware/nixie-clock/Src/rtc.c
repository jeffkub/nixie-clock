#include "rtc.h"

#include <string.h>

#include "FreeRTOS.h"
#include "portmacro.h"
#include "semphr.h"

#include "stm32f3xx_hal.h"

#define UNLOCK_WRITE()  WRITE_REG(RTC->WPR, 0xCA); \
                        WRITE_REG(RTC->WPR, 0x53);

#define LOCK_WRITE()    WRITE_REG(RTC->WPR, 0xFF);

static SemaphoreHandle_t wakeupSem;

static uint32_t bcdToByte(uint32_t val);
static uint32_t byteToBcd(uint32_t val);

static uint32_t bcdToByte(uint32_t val)
{
    return (((val & 0xF0) >> 4) * 10) + (val & 0x0F);
}

static uint32_t byteToBcd(uint32_t val)
{
    val &= 0xFF;

    return ((val / 10) << 4) | (val % 10);
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

    UNLOCK_WRITE();

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

    /* Enable time stamp on rising edge */
    CLEAR_BIT(RTC->CR, RTC_CR_TSEDGE);
    SET_BIT(RTC->CR, RTC_CR_TSE);

    LOCK_WRITE();

    return;
}

void rtc_wait(void)
{
    xSemaphoreTake(wakeupSem, portMAX_DELAY);

    return;
}

time_t rtc_getTime(void)
{
    struct tm ts = {0};
    uint32_t  tr;
    uint32_t  dr;

    /* Read time and date registers */
    while(!READ_BIT(RTC->ISR, RTC_ISR_RSF));
    tr = READ_REG(RTC->TR);
    dr = READ_REG(RTC->DR);
    CLEAR_BIT(RTC->ISR, RTC_ISR_RSF);

    /* Process registers */
    ts.tm_sec  = bcdToByte((tr & (RTC_TR_ST  | RTC_TR_SU )) >> RTC_TR_SU_Pos );
    ts.tm_min  = bcdToByte((tr & (RTC_TR_MNT | RTC_TR_MNU)) >> RTC_TR_MNU_Pos);
    ts.tm_hour = bcdToByte((tr & (RTC_TR_HT  | RTC_TR_HU )) >> RTC_TR_HU_Pos );

    ts.tm_mday = bcdToByte((dr & (RTC_DR_DT  | RTC_DR_DU )) >> RTC_DR_DU_Pos );
    ts.tm_mon  = bcdToByte((dr & (RTC_DR_MT  | RTC_DR_MU )) >> RTC_DR_MU_Pos ) - 1;
    ts.tm_year = bcdToByte((dr & (RTC_DR_YT  | RTC_DR_YU )) >> RTC_DR_YU_Pos ) + 100;

    return mktime(&ts);
}

int rtc_setTime(time_t time)
{
    struct tm ts = {0};
    uint32_t  tr = 0;
    uint32_t  dr = 0;

    localtime_r(&time, &ts);

    tr |= byteToBcd(ts.tm_sec       ) << RTC_TR_SU_Pos;
    tr |= byteToBcd(ts.tm_min       ) << RTC_TR_MNU_Pos;
    tr |= byteToBcd(ts.tm_hour      ) << RTC_TR_HU_Pos;

    dr |= byteToBcd(ts.tm_mday      ) << RTC_DR_DU_Pos;
    dr |= byteToBcd(ts.tm_mon  + 1  ) << RTC_DR_MU_Pos;
    dr |= byteToBcd(ts.tm_wday + 1  ) << RTC_DR_WDU_Pos;
    dr |= byteToBcd(ts.tm_year - 100) << RTC_DR_YU_Pos;

    UNLOCK_WRITE();

    /* Enter initialization mode */
    SET_BIT(RTC->ISR, RTC_ISR_INIT);
    while(!READ_BIT(RTC->ISR, RTC_ISR_INITF));

    /* Load default date and time regs */
    WRITE_REG(RTC->TR, tr);
    WRITE_REG(RTC->DR, dr);

    /* Exit initialization mode */
    CLEAR_BIT(RTC->ISR, RTC_ISR_INIT);

    LOCK_WRITE();

    return 0;
}

int32_t rtc_getTsOffset(void)
{
    uint32_t ssr;

    if(!READ_BIT(RTC->ISR, RTC_ISR_TSF))
    {
        /* No timestamp event available */
        return -1;
    }

    /* Read and clear timestamp sub-second register */
    ssr = READ_REG(RTC->TSSSR);
    CLEAR_BIT(RTC->ISR, RTC_ISR_TSF);

    if(READ_BIT(RTC->ISR, RTC_ISR_TSOVF))
    {
        /* Timestamp overflow error */
        CLEAR_BIT(RTC->ISR, RTC_ISR_TSOVF);
        return -1;
    }

    return (int32_t)ssr;
}

void rtc_adjust(uint32_t offset, bool advance)
{
    uint32_t shiftr;

    if(offset == 0)
    {
        /* Nothing to do */
        return;
    }

    if(READ_BIT(RTC->SSR, 1 << 15))
    {
        return;
    }

    shiftr = offset & RTC_SHIFTR_SUBFS;
    shiftr |= advance ? RTC_SHIFTR_ADD1S : 0;

    UNLOCK_WRITE();

    /* Perform time shift */
    WRITE_REG(RTC->SHIFTR, shiftr);

    LOCK_WRITE();

    return;
}
