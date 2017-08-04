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
#include "uart3.h"

#include "globals.h"

#include "stm32f3xx_hal.h"
#include "FreeRTOS.h"
#include "portmacro.h"
#include "semphr.h"


/* Private definitions ********************************************************/
#define UART_INST USART3

#define UART_DIV_SAMPLING16(__PCLK__, __BAUD__)  (((__PCLK__) + ((__BAUD__)/2U)) / (__BAUD__))

#define BUFFER_SIZE 256


/* Private variables **********************************************************/
static SemaphoreHandle_t dev_mutex;
static SemaphoreHandle_t done_sem;

static char   rxBuf[BUFFER_SIZE];
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
    isr_flags = READ_REG(UART_INST->ISR);

    /* RX interrupt */
    if(isr_flags & USART_ISR_RXNE)
    {
        data = (char) READ_REG(UART_INST->RDR);

        if((rxBufWritePtr - rxBufReadPtr) < BUFFER_SIZE)
        {
            rxBuf[rxBufWritePtr % BUFFER_SIZE] = data;
            rxBufWritePtr++;
        }

        xSemaphoreGiveFromISR(done_sem, &task_woken);
    }

    /* Acknowledge interrupts */
    WRITE_REG(UART_INST->ICR, isr_flags);

    portYIELD_FROM_ISR(task_woken);

    return;
}

void uart3_init(void)
{
    GPIO_InitTypeDef gpio;

    dev_mutex = xSemaphoreCreateMutex();
    done_sem  = xSemaphoreCreateBinary();

    /* Peripheral clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();

    /**USART3 GPIO Configuration
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX
    */
    gpio.Pin = GPIO_PIN_10|GPIO_PIN_11;
    gpio.Mode = GPIO_MODE_AF_PP;
    gpio.Pull = GPIO_PULLUP;
    gpio.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOB, &gpio);

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);

    /* Disable the UART */
    CLEAR_BIT(UART_INST->CR1, USART_CR1_UE);

    /* 8-bit word length, no parity */
    CLEAR_BIT(UART_INST->CR1, USART_CR1_M1 | USART_CR1_M0 | USART_CR1_PCE | USART_CR1_PS);

    /* Enable TX and RX */
    SET_BIT(UART_INST->CR1, USART_CR1_TE | USART_CR1_RE);

    /* 1 stop bit */
    CLEAR_BIT(UART_INST->CR2, USART_CR2_STOP_1 | USART_CR2_STOP_0);

    /* Set the baud rate */
    UART_INST->BRR = (uint16_t)(UART_DIV_SAMPLING16(HAL_RCC_GetPCLK1Freq(), 9600));

    /* Enable RX interrupt */
    CLEAR_BIT(UART_INST->CR1, USART_CR1_TXEIE);
    SET_BIT(UART_INST->CR1, USART_CR1_RXNEIE);

    /* Enable the UART */
    SET_BIT(UART_INST->CR1, USART_CR1_UE);

    return;
}

ssize_t uart3_read(void * buf, size_t nbyte)
{
    ssize_t read;

    for(read = 0; read < nbyte; read++)
    {
        /* Wait for character to read */
        while(rxBufReadPtr == rxBufWritePtr)
        {
            xSemaphoreTake(done_sem, portMAX_DELAY);
        }

        ((char*)buf)[read] = rxBuf[rxBufReadPtr % BUFFER_SIZE];
        rxBufReadPtr++;
    }

    return read;
}

char * uart3_gets(char * str, size_t maxlen)
{
    size_t read = 0;

    if(str == NULL || maxlen == 0)
    {
        return NULL;
    }

    while(read < (maxlen - 1)) /* -1 for NULL char */
    {
        if(uart3_read(&str[read], 1) < 0)
        {
            /* Read error */
            return NULL;
        }

        if(read == 0 && isspace((int)str[read]) != 0)
        {
            /* Skip leading whitespace */
            continue;
        }
        else if(str[read] == '\r' || str[read] == '\n')
        {
            /* Reached end of line */
            read++;
            break;
        }

        read++;
    }

    /* Null terminate the string */
    str[read] = '\0';

    return str;
}
