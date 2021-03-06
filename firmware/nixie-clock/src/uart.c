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
#include "uart.h"

#include "globals.h"
#include "debug.h"
#include "stm32f3xx_hal.h"
#include "FreeRTOS.h"
#include "portmacro.h"
#include "semphr.h"


/* Private definitions ********************************************************/
#define UART_DEV    USART3

#define UART_DIV_SAMPLING16(__PCLK__, __BAUD__)  (((__PCLK__) + ((__BAUD__)/2U)) / (__BAUD__))

#define BUF_SIZE    256
#define BUF_MASK    (BUF_SIZE-1)
#define BUF_IDX(x)  ((x) & BUF_MASK)


/* Private variables **********************************************************/
static SemaphoreHandle_t mutex;
static SemaphoreHandle_t rxActSem;

static char   rxBuf[BUF_SIZE];
static size_t rxBufReadPtr  = 0;
static size_t rxBufWritePtr = 0;


/* Private function prototypes ************************************************/


/* Private function definitions ***********************************************/


/* Public function definitions ************************************************/
void USART3_IRQHandler(void)
{
    BaseType_t task_woken = pdFALSE;
    uint32_t   isr_flags;
    uint32_t   data;

    /* Read interrupt flags */
    isr_flags = READ_REG(UART_DEV->ISR);

    /* RX interrupt */
    if(isr_flags & USART_ISR_RXNE)
    {
        data = (char) READ_REG(UART_DEV->RDR);

        if((rxBufWritePtr - rxBufReadPtr) < BUF_SIZE)
        {
            rxBuf[BUF_IDX(rxBufWritePtr)] = data;
            rxBufWritePtr++;
        }

        xSemaphoreGiveFromISR(rxActSem, &task_woken);
    }

    /* Acknowledge interrupts */
    WRITE_REG(UART_DEV->ICR, isr_flags);

    portYIELD_FROM_ISR(task_woken);

    return;
}

void uart_init(void)
{
    mutex = xSemaphoreCreateMutex();
    debug_assert(mutex);

    rxActSem = xSemaphoreCreateBinary();
    debug_assert(rxActSem);

    /* Peripheral clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(USART3_IRQn, USART3_IRQ_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);

    /* UART configuration */
    WRITE_REG(UART_DEV->CR1, USART_CR1_TE | USART_CR1_RE);

    WRITE_REG(UART_DEV->CR2, 0);

    WRITE_REG(UART_DEV->BRR, UART_DIV_SAMPLING16(HAL_RCC_GetPCLK1Freq(), 9600));

    /* Enable UART peripheral */
    SET_BIT(UART_DEV->CR1, USART_CR1_UE);

    /* Enable RX interrupt */
    SET_BIT(UART_DEV->CR1, USART_CR1_RXNEIE);

    return;
}

char * uart_gets(char * str, size_t maxlen)
{
    size_t read = 0;

    if(str == NULL || maxlen == 0)
    {
        return NULL;
    }

    xSemaphoreTake(mutex, portMAX_DELAY);

    while(read < (maxlen - 1)) /* -1 for NULL char */
    {
        while(rxBufReadPtr == rxBufWritePtr)
        {
            xSemaphoreTake(rxActSem, portMAX_DELAY);
        }

        str[read] = rxBuf[BUF_IDX(rxBufReadPtr)];
        rxBufReadPtr++;

        if(read == 0 && isspace((int)str[read]) != 0)
        {
            /* Skip leading whitespace */
            continue;
        }
        else if(str[read] == '\n')
        {
            /* Reached end of line */
            read++;
            break;
        }

        read++;
    }

    xSemaphoreGive(mutex);

    /* Null terminate the string */
    str[read] = '\0';

    return str;
}
