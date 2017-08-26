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
#include "nixieDriver.h"

#include "globals.h"

#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "semphr.h"

#include "debug.h"
#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_spi.h"


/* Global variables ***********************************************************/


/* Private definitions ********************************************************/
#define TUBE_CNT     6
#define DIGIT_CNT   10

#define SHIFT_BYTES  8
#define BIT_CNT      (SHIFT_BYTES * 8)


/* Private variables **********************************************************/
static const uint8_t m_nixieBitmap[TUBE_CNT][DIGIT_CNT] =
{
    { 10,  9,  8,  7, 26, 25, 24, 23, 22, 11 },
    { 15, 14, 13, 12, 21, 20, 19, 18, 17, 16 },

    {  2,  1,  0, 31, 30, 29, 28, 27,  4,  3 },
    { 62, 61, 60, 59, 36, 35, 34, 33, 32, 63 },

    { 51, 50, 49, 48, 47, 46, 45, 44, 53, 52 },
    { 56, 55, 54, 43, 42, 41, 40, 39, 58, 57 }
};

static SemaphoreHandle_t dev_mutex;
static SemaphoreHandle_t done_sem;


/* Private function prototypes ************************************************/
static void hvEnable(bool state);
static void dispEnable(bool state);
static void latchEnable(bool state);

static void clrbit(void* addr, unsigned int cnt);
static void spiTransmit(const void* data, size_t len);


/* Private function definitions ***********************************************/
static void hvEnable(bool state)
{
    if(state)
    {
        HAL_GPIO_WritePin(HV_EN_GPIO_Port, HV_EN_Pin, GPIO_PIN_SET);
    }
    else
    {
        HAL_GPIO_WritePin(HV_EN_GPIO_Port, HV_EN_Pin, GPIO_PIN_RESET);
    }
}

static void dispEnable(bool state)
{
    /* Output is inverted */
    if(state)
    {
        HAL_GPIO_WritePin(DISP_BL_GPIO_Port, DISP_BL_Pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(DISP_BL_GPIO_Port, DISP_BL_Pin, GPIO_PIN_SET);
    }

    return;
}

static void latchEnable(bool state)
{
    /* Output is inverted */
    if(state)
    {
        HAL_GPIO_WritePin(DISP_LE_GPIO_Port, DISP_LE_Pin, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(DISP_LE_GPIO_Port, DISP_LE_Pin, GPIO_PIN_SET);
    }

    return;
}

static void clrbit(void* addr, unsigned int cnt)
{
    uint8_t*     byte = ((uint8_t*)addr) + (cnt / 8);
    unsigned int bit = cnt % 8;

    *byte &= ~(1 << bit);

    return;
}

static void spiTransmit(const void* data, size_t len)
{
#if 0
    /* Start transmit */
    if(HAL_SPI_Transmit_IT(&hspi2, (uint8_t*)data, len) != HAL_OK)
    {
        debug_printf("SPI transmit failed\n");

        return;
    }
#endif
    /* Wait for transmit to complete */
    xSemaphoreTake(done_sem, portMAX_DELAY);

    return;
}


/* Public function definitions ************************************************/
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    BaseType_t taskWoken;

    xSemaphoreGiveFromISR(done_sem, &taskWoken);

    portYIELD_FROM_ISR(taskWoken);

    return;
}

void nixieDriver_init(void)
{
    dev_mutex = xSemaphoreCreateMutex();
    done_sem  = xSemaphoreCreateBinary();

    hvEnable(true);
    dispEnable(false);

    return;
}

void nixieDriver_set(int* vals)
{
    int     index;
    uint8_t bitmask[SHIFT_BYTES];
    int     tensDigit;
    int     onesDigit;

    xSemaphoreTake(dev_mutex, portMAX_DELAY);

    memset(bitmask, 0xFF, sizeof(bitmask));

    for(index = 0; index < NUM_CNT; index++)
    {
        tensDigit = vals[index] / DIGIT_CNT;
        onesDigit = vals[index] % DIGIT_CNT;

        if(tensDigit >= DIGIT_CNT)
        {
            tensDigit = DIGIT_CNT-1;
        }

        clrbit(bitmask, m_nixieBitmap[index*2][tensDigit]);
        clrbit(bitmask, m_nixieBitmap[index*2 + 1][onesDigit]);
    }

    latchEnable(false);

    vTaskDelay(2);

    spiTransmit(bitmask, SHIFT_BYTES);

    vTaskDelay(2);

    latchEnable(true);

    vTaskDelay(2);

    dispEnable(true);

    xSemaphoreGive(dev_mutex);

    return;
}
