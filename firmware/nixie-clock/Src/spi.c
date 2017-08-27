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

#include "FreeRTOS.h"
#include "semphr.h"


/* Private definitions ********************************************************/
#define SPIDEV SPI2

typedef struct spiTransfer
{
    size_t    len;

    uint8_t * txData;
    size_t    txOffset;

    uint8_t * rxData;
    size_t    rxOffset;
} spiTransfer_t;


/* Private variables **********************************************************/
static SemaphoreHandle_t mutex;
static SemaphoreHandle_t doneSem;

static spiTransfer_t xfer;


/* Private function prototypes ************************************************/


/* Private function definitions ***********************************************/


/* Public function definitions ************************************************/
void SPI2_IRQHandler(void)
{
    BaseType_t task_woken = pdFALSE;
    uint8_t read;

    while(READ_BIT(SPIDEV->SR, SPI_SR_RXNE) && (xfer.rxOffset < xfer.len))
    {
        read = *(__IO uint8_t *)&SPIDEV->DR;

        if(xfer.rxData)
        {
            xfer.rxData[xfer.rxOffset] = read;
        }

        xfer.rxOffset++;
    }

    while(READ_BIT(SPIDEV->SR, SPI_SR_TXE) && (xfer.txOffset < xfer.len))
    {
        *(__IO uint8_t *)&SPIDEV->DR = xfer.txData[xfer.txOffset];
        xfer.txOffset++;
    }

    if(xfer.txOffset >= xfer.len)
    {
        /* Transmit complete */
        CLEAR_BIT(SPIDEV->CR2, SPI_CR2_TXEIE);
    }

    if(xfer.rxOffset >= xfer.len)
    {
        /* Transfer complete */
        CLEAR_BIT(SPIDEV->CR2, SPI_CR2_RXNEIE);

        xSemaphoreGiveFromISR(doneSem, &task_woken);
    }

    portYIELD_FROM_ISR(task_woken);

	return;
}

void spi_init(void)
{
    mutex = xSemaphoreCreateMutex();
    doneSem = xSemaphoreCreateBinary();

    /* Peripheral clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();

    /* Peripheral interrupt init */
    HAL_NVIC_SetPriority(SPI2_IRQn, SPI2_IRQ_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(SPI2_IRQn);

    /* SPI configuration */
    WRITE_REG(SPIDEV->CR1,
        SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_LSBFIRST | (7 << SPI_CR1_BR_Pos) |
        SPI_CR1_MSTR | SPI_CR1_CPOL | SPI_CR1_CPHA);

    WRITE_REG(SPIDEV->CR2, SPI_CR2_FRXTH | (7 << SPI_CR2_DS_Pos));

    /* Enable SPI peripheral */
    SET_BIT(SPIDEV->CR1, SPI_CR1_SPE);

    return;
}

int spi_tx(const void * data, size_t len)
{
    if(len <= 0)
    {
        return 0;
    }

    xSemaphoreTake(mutex, portMAX_DELAY);

    /* Prepare SPI transfer */
    xfer.len      = len;
    xfer.txData   = (uint8_t *)data;
    xfer.txOffset = 0;
    xfer.rxData   = NULL;
    xfer.rxOffset = 0;

    /* Enable interrupts */
    SET_BIT(SPIDEV->CR2, SPI_CR2_TXEIE | SPI_CR2_RXNEIE);

    /* Wait for transfer to complete */
    xSemaphoreTake(doneSem, portMAX_DELAY);

    xSemaphoreGive(mutex);

    return 0;
}
