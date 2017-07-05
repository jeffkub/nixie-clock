#include "uart3.h"

#include "stm32f3xx.h"
#include "stm32f3xx_hal_rcc.h"

#define UART_INST USART3

#define UART_DIV_SAMPLING16(__PCLK__, __BAUD__)  (((__PCLK__) + ((__BAUD__)/2U)) / (__BAUD__))

void uart3_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
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

    /* Enable the UART */
    SET_BIT(UART_INST->CR1, USART_CR1_UE);

    return;
}

ssize_t uart3_read(void * buf, size_t nbyte)
{
    return 0;
}
