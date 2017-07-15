#include "uart3.h"

#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "semphr.h"

#include "stm32f3xx.h"
#include "stm32f3xx_hal_rcc.h"

#define UART_INST USART3

#define UART_DIV_SAMPLING16(__PCLK__, __BAUD__)  (((__PCLK__) + ((__BAUD__)/2U)) / (__BAUD__))

#define BUFFER_SIZE 2048

static SemaphoreHandle_t devMutex;
static SemaphoreHandle_t doneSem;

static char   rxBuf[BUFFER_SIZE];
static size_t rxBufReadPtr  = 0;
static size_t rxBufWritePtr = 0;

void USART3_IRQHandler(void)
{
    BaseType_t taskWoken = pdFALSE;
    uint32_t   isrflags;
    uint32_t   rxData;

    /* Read interrupt flags */
    isrflags = READ_REG(UART_INST->ISR);

    /* RX interrupt */
    if(isrflags & USART_ISR_RXNE)
    {
        rxData = (char) READ_REG(UART_INST->RDR);

        if((rxBufWritePtr - rxBufReadPtr) < BUFFER_SIZE)
        {
            rxBuf[rxBufWritePtr % BUFFER_SIZE] = rxData;
            rxBufWritePtr++;
        }

        xSemaphoreGiveFromISR(doneSem, &taskWoken);
    }
    
    /* Acknowledge interrupts */
    WRITE_REG(UART_INST->ICR, isrflags);

    portYIELD_FROM_ISR(taskWoken);

    return;
}

void uart3_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    devMutex = xSemaphoreCreateMutex();
    doneSem  = xSemaphoreCreateBinary();

    /* Peripheral clock enable */
    __HAL_RCC_USART3_CLK_ENABLE();
  
    /**USART3 GPIO Configuration    
    PB10     ------> USART3_TX
    PB11     ------> USART3_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
            xSemaphoreTake(doneSem, portMAX_DELAY);
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

        if(read == 0 && isspace(str[read]) != 0)
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
