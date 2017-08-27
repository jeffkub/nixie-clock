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
#include "pwm.h"

#include "globals.h"
#include "debug.h"
#include "stm32f3xx_hal.h"
#include "FreeRTOS.h"
#include "semphr.h"


/* Private definitions ********************************************************/
#define TIM_DEV     TIM2
#define PRESCALER   0
#define PERIOD      0xFFFF


/* Private variables **********************************************************/
static SemaphoreHandle_t timerSem;


/* Private function prototypes ************************************************/


/* Private function definitions ***********************************************/


/* Public function definitions ************************************************/
void TIM2_IRQHandler(void)
{
    BaseType_t taskWoken = pdFALSE;
    uint32_t status;

    status = READ_REG(TIM_DEV->SR) & READ_REG(TIM_DEV->DIER);

    if(status & TIM_SR_UIF)
    {
        xSemaphoreGiveFromISR(timerSem, &taskWoken);
    }

    /* Acknowledge interrupts */
    WRITE_REG(TIM_DEV->SR, ~status);

    portYIELD_FROM_ISR(taskWoken);

    return;
}

void pwm_init(void)
{
    timerSem = xSemaphoreCreateBinary();
    debug_assert(timerSem);

    /* Peripheral clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(TIM2_IRQn, TIM2_IRQ_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

    /* Timer configuration */
    WRITE_REG(TIM_DEV->CR1, TIM_CR1_ARPE);

    WRITE_REG(TIM_DEV->PSC, PRESCALER);

    WRITE_REG(TIM_DEV->ARR, PERIOD);

    WRITE_REG(TIM_DEV->CCMR1,
        (6 << TIM_CCMR1_OC2M_Pos) | TIM_CCMR1_OC2PE | (0 << TIM_CCMR1_CC2S_Pos) |
        (6 << TIM_CCMR1_OC1M_Pos) | TIM_CCMR1_OC1PE | (0 << TIM_CCMR1_CC1S_Pos));

    WRITE_REG(TIM_DEV->CCMR2,
        (6 << TIM_CCMR2_OC4M_Pos) | TIM_CCMR2_OC4PE | (0 << TIM_CCMR2_CC4S_Pos) |
        (6 << TIM_CCMR2_OC3M_Pos) | TIM_CCMR2_OC3PE | (0 << TIM_CCMR2_CC3S_Pos));

    /* Initialize OC values */
    WRITE_REG(TIM_DEV->CCR1, 0);
    WRITE_REG(TIM_DEV->CCR2, 0);
    WRITE_REG(TIM_DEV->CCR3, 0);
    WRITE_REG(TIM_DEV->CCR4, 0);

    /* Re-initialize counter and update registers */
    WRITE_REG(TIM_DEV->EGR, TIM_EGR_UG);

    /* Enable OC channels */
    WRITE_REG(TIM_DEV->CCER,
        TIM_CCER_CC4E | TIM_CCER_CC3E | TIM_CCER_CC2E | TIM_CCER_CC1E);

    /* Enable update interrupt */
    SET_BIT(TIM_DEV->DIER, TIM_DIER_UIE);

    /* Enable the timer */
    SET_BIT(TIM_DEV->CR1, TIM_CR1_CEN);

    return;
}

void pwm_wait(void)
{
    xSemaphoreTake(timerSem, portMAX_DELAY);

    return;
}

int pwm_set(unsigned channel, unsigned value)
{
    switch(channel)
    {
        case 1:
        {
            WRITE_REG(TIM_DEV->CCR1, value);
            break;
        }
        case 2:
        {
            WRITE_REG(TIM_DEV->CCR2, value);
            break;
        }
        case 3:
        {
            WRITE_REG(TIM_DEV->CCR3, value);
            break;
        }
        case 4:
        {
            WRITE_REG(TIM_DEV->CCR4, value);
            break;
        }
        default:
        {
            return -1;
        }
    }

    return 0;
}
