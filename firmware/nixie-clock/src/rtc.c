/*******************************************************************************
MIT License

Copyright (c) 2017 Jeff Kubascik

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

/* Includes *******************************************************************/
#include "rtc.h"

#include "globals.h"

#include "debug.h"
#include "stm32f3xx_hal.h"
#include "FreeRTOS.h"
#include "portmacro.h"
#include "semphr.h"
#include "task.h"


/* Private definitions ********************************************************/
#define UNLOCK_WRITE()  WRITE_REG(RTC->WPR, 0xCA); \
                        WRITE_REG(RTC->WPR, 0x53);

#define LOCK_WRITE()    WRITE_REG(RTC->WPR, 0xFF);


/* Private variables **********************************************************/
static SemaphoreHandle_t wakeupSem;


/* Private function prototypes ************************************************/
static inline void readTimeRegs(uint32_t * dr, uint32_t * tr, uint32_t * ssr);

static uint32_t bcdToByte(uint32_t val);
static uint32_t byteToBcd(uint32_t val);


/* Private function definitions ***********************************************/

/* Since the shadow registers are disabled, we must ensure consistency while
 * reading the 3 time registers.  So we read the sub-second register twice,
 * before and after reading the time and date registers.  If the sub-second
 * register changed, we re-read the time and date registers. */
static inline void readTimeRegs(uint32_t * dr, uint32_t * tr, uint32_t * ssr)
{
    uint32_t temp;

    *ssr = READ_REG(RTC->SSR);

    while(1)
    {
        *tr = READ_REG(RTC->TR);
        *dr = READ_REG(RTC->DR);

        temp = READ_REG(RTC->SSR);
        if(temp == *ssr)
        {
            break;
        }

        *ssr = temp;
    };

    return;
}

static uint32_t bcdToByte(uint32_t val)
{
    return (((val & 0xF0) >> 4) * 10) + (val & 0x0F);
}

static uint32_t byteToBcd(uint32_t val)
{
    val &= 0xFF;

    return ((val / 10) << 4) | (val % 10);
}


/* Public function definitions ************************************************/
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
    debug_assert(wakeupSem);

    /* Peripheral clock enable */
    __HAL_RCC_RTC_ENABLE();

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, RTC_WKUP_IRQ_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);

    UNLOCK_WRITE();

    /* Check if RTC calendar needs to be initialized */
    if(!READ_BIT(RTC->ISR, RTC_ISR_INITS))
    {
        /* Enter initialization mode */
        SET_BIT(RTC->ISR, RTC_ISR_INIT);
        while(!READ_BIT(RTC->ISR, RTC_ISR_INITF));

        /* Program the prescalar values */
        WRITE_REG(RTC->PRER,
            (PREDIV_A << RTC_PRER_PREDIV_A_Pos) |
            (PREDIV_S << RTC_PRER_PREDIV_S_Pos));

        /* Configure for 24h time */
        CLEAR_BIT(RTC->CR, RTC_CR_FMT);

        /* Load default date and time regs */
        WRITE_REG(RTC->TR, 0x00000000);
        WRITE_REG(RTC->DR, 0x00172101);

        /* Exit initialization mode */
        CLEAR_BIT(RTC->ISR, RTC_ISR_INIT);
    }

    /* Bypass the shadow registers */
    SET_BIT(RTC->CR, RTC_CR_BYPSHAD);

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

    /* Disable time stamp feature - using ISR on pps pin instead */
    CLEAR_BIT(RTC->CR, RTC_CR_TSE);

    LOCK_WRITE();

    return;
}

void rtc_wait(void)
{
    xSemaphoreTake(wakeupSem, portMAX_DELAY);

    return;
}

void rtc_getTime(struct tm * ts, int * subsec)
{
    static const struct tm zeroTime = {0};
    uint32_t dr;
    uint32_t tr;
    uint32_t ssr;

    /* Atomically read the time and date registers */
    taskENTER_CRITICAL();
    readTimeRegs(&dr, &tr, &ssr);
    taskEXIT_CRITICAL();

    /* Fill out the time struct */
    *ts = zeroTime;

    ts->tm_sec  = bcdToByte((tr & (RTC_TR_ST  | RTC_TR_SU )) >> RTC_TR_SU_Pos );
    ts->tm_min  = bcdToByte((tr & (RTC_TR_MNT | RTC_TR_MNU)) >> RTC_TR_MNU_Pos);
    ts->tm_hour = bcdToByte((tr & (RTC_TR_HT  | RTC_TR_HU )) >> RTC_TR_HU_Pos );

    ts->tm_mday = bcdToByte((dr & (RTC_DR_DT  | RTC_DR_DU )) >> RTC_DR_DU_Pos );
    ts->tm_mon  = bcdToByte((dr & (RTC_DR_MT  | RTC_DR_MU )) >> RTC_DR_MU_Pos ) - 1;
    ts->tm_wday = bcdToByte((dr & (RTC_DR_WDU             )) >> RTC_DR_WDU_Pos) - 1;
    ts->tm_year = bcdToByte((dr & (RTC_DR_YT  | RTC_DR_YU )) >> RTC_DR_YU_Pos ) + 100;

    if(subsec)
    {
        *subsec = PREDIV_S - (int)ssr;
    }

    return;
}

void rtc_getTimeFromISR(struct tm * ts, int * subsec)
{
    static const struct tm zeroTime = {0};
    UBaseType_t isrStatus;
    uint32_t dr;
    uint32_t tr;
    uint32_t ssr;

    /* Atomically read the time and date registers */
    isrStatus = taskENTER_CRITICAL_FROM_ISR();
    readTimeRegs(&dr, &tr, &ssr);
    taskEXIT_CRITICAL_FROM_ISR(isrStatus);

    /* Fill out the time struct */
    *ts = zeroTime;

    ts->tm_sec  = bcdToByte((tr & (RTC_TR_ST  | RTC_TR_SU )) >> RTC_TR_SU_Pos );
    ts->tm_min  = bcdToByte((tr & (RTC_TR_MNT | RTC_TR_MNU)) >> RTC_TR_MNU_Pos);
    ts->tm_hour = bcdToByte((tr & (RTC_TR_HT  | RTC_TR_HU )) >> RTC_TR_HU_Pos );

    ts->tm_mday = bcdToByte((dr & (RTC_DR_DT  | RTC_DR_DU )) >> RTC_DR_DU_Pos );
    ts->tm_mon  = bcdToByte((dr & (RTC_DR_MT  | RTC_DR_MU )) >> RTC_DR_MU_Pos ) - 1;
    ts->tm_wday = bcdToByte((dr & (RTC_DR_WDU             )) >> RTC_DR_WDU_Pos) - 1;
    ts->tm_year = bcdToByte((dr & (RTC_DR_YT  | RTC_DR_YU )) >> RTC_DR_YU_Pos ) + 100;

    if(subsec)
    {
        *subsec = PREDIV_S - (int)ssr;
    }

    return;
}

void rtc_setTime(const struct tm * ts)
{
    uint32_t tr = 0;
    uint32_t dr = 0;

    /* Determine the time and date register values */
    tr |= byteToBcd(ts->tm_sec       ) << RTC_TR_SU_Pos;
    tr |= byteToBcd(ts->tm_min       ) << RTC_TR_MNU_Pos;
    tr |= byteToBcd(ts->tm_hour      ) << RTC_TR_HU_Pos;

    dr |= byteToBcd(ts->tm_mday      ) << RTC_DR_DU_Pos;
    dr |= byteToBcd(ts->tm_mon  + 1  ) << RTC_DR_MU_Pos;
    dr |= byteToBcd(ts->tm_wday + 1  ) << RTC_DR_WDU_Pos;
    dr |= byteToBcd(ts->tm_year - 100) << RTC_DR_YU_Pos;

    taskENTER_CRITICAL();
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
    taskEXIT_CRITICAL();

    return;
}

void rtc_adjust(int offset)
{
    uint32_t shiftr;

    if(offset == 0)
    {
        /* Nothing to do */
        return;
    }

    if(READ_BIT(RTC->SSR, 1 << 15))
    {
        /* Shift operation pending */
        return;
    }

    if(offset < 0)
    {
        /* Subtract a fraction of a second */
        shiftr = (-offset) & RTC_SHIFTR_SUBFS;
    }
    else
    {
        /* Add a fraction of a second */
        shiftr = (PREDIV_S + 1 - offset) & RTC_SHIFTR_SUBFS;
        shiftr |= RTC_SHIFTR_ADD1S;
    }

    taskENTER_CRITICAL();
    UNLOCK_WRITE();

    /* Perform time shift */
    WRITE_REG(RTC->SHIFTR, shiftr);

    LOCK_WRITE();
    taskEXIT_CRITICAL();

    return;
}
