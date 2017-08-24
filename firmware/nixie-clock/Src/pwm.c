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
#include "stm32f3xx_hal.h"


/* Private definitions ********************************************************/
#define TIMER       TIM2
#define PRESCALER   0
#define PERIOD      0xFFFF


/* Private variables **********************************************************/


/* Private function prototypes ************************************************/


/* Private function definitions ***********************************************/


/* Public function definitions ************************************************/
void pwm_init(void)
{
    /* Peripheral clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();

    /* Timer configuration */
    WRITE_REG(TIMER->CR1, TIM_CR1_ARPE);

    /* Timer prescaler */
    WRITE_REG(TIMER->PSC, PRESCALER);

    /* Timer period */
    WRITE_REG(TIMER->ARR, PERIOD);

    /* Re-initialize counter and update registers */
    WRITE_REG(TIMER->EGR, TIM_EGR_UG);

    /* Output compare enable for all 4 channels */
    WRITE_REG(TIMER->CCMR1,
        (6 << TIM_CCMR1_OC2M_Pos) | TIM_CCMR1_OC2PE | (0 << TIM_CCMR1_CC2S_Pos) |
        (6 << TIM_CCMR1_OC1M_Pos) | TIM_CCMR1_OC1PE | (0 << TIM_CCMR1_CC1S_Pos));
    WRITE_REG(TIMER->CCMR2,
        (6 << TIM_CCMR2_OC4M_Pos) | TIM_CCMR2_OC4PE | (0 << TIM_CCMR2_CC4S_Pos) |
        (6 << TIM_CCMR2_OC3M_Pos) | TIM_CCMR2_OC3PE | (0 << TIM_CCMR2_CC3S_Pos));

    /* Initialize OC values */
    WRITE_REG(TIMER->CCR1, 0);
    WRITE_REG(TIMER->CCR2, 0);
    WRITE_REG(TIMER->CCR3, 0);
    WRITE_REG(TIMER->CCR4, 0);

    /* Enable OC channels */
    WRITE_REG(TIMER->CCER,
        TIM_CCER_CC4E | TIM_CCER_CC3E | TIM_CCER_CC2E | TIM_CCER_CC1E);

    /* Enable the timer */
    SET_BIT(TIMER->CR1, TIM_CR1_CEN);

    return;
}

int pwm_set(unsigned channel, unsigned value)
{
    switch(channel)
    {
        case 1:
        {
            TIMER->CCR1 = value;
            break;
        }
        case 2:
        {
            TIMER->CCR2 = value;
            break;
        }
        case 3:
        {
            TIMER->CCR3 = value;
            break;
        }
        case 4:
        {
            TIMER->CCR4 = value;
            break;
        }
        default:
        {
            return -1;
        }
    }

    return 0;
}
