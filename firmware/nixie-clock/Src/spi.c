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
#include "spi.h"

#include "globals.h"
#include "stm32f3xx_hal.h"


/* Private definitions ********************************************************/


/* Private variables **********************************************************/


/* Private function prototypes ************************************************/


/* Private function definitions ***********************************************/


/* Public function definitions ************************************************/
void SPI2_IRQHandler(void)
{
	return;
}

void spi_init(void)
{
    /* Peripheral clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(SPI2_IRQn, SPI2_IRQ_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(SPI2_IRQn);

    return;
}
