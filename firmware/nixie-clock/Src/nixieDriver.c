#include "nixieDriver.h"

#include <stdbool.h>
#include <string.h>

#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "semphr.h"

#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_spi.h"

#define TUBE_CNT     6
#define DIGIT_CNT   10

#define SHIFT_BYTES  8
#define BIT_CNT      (SHIFT_BYTES * 8)

extern SPI_HandleTypeDef hspi2;

static const uint8_t m_nixieBitmap[TUBE_CNT][DIGIT_CNT] =
{
    { 10,  9,  8,  7, 26, 25, 24, 23, 22, 11 },
    { 15, 14, 13, 12, 21, 20, 19, 18, 17, 16 },

    {  2,  1,  0, 31, 30, 29, 28, 27,  4,  3 },
    { 62, 61, 60, 59, 36, 35, 34, 33, 32, 63 },

    { 51, 50, 49, 48, 47, 46, 45, 44, 53, 52 },
    { 56, 55, 54, 43, 42, 41, 40, 39, 58, 57 }
};

static SemaphoreHandle_t devMutex;
static SemaphoreHandle_t doneSem;

static void setDispEnable(bool state);
static void setLatchEnable(bool state);
static void clrbit(void* addr, unsigned int cnt);
static void spiTransmit(const void* data, size_t len);

static void setDispEnable(bool state)
{
    /* Output is inverted */
    if(state)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
    }
    
    return;
}

static void setLatchEnable(bool state)
{
    /* Output is inverted */
    if(state)
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
    }

    return;
}

static void clrbit(void* addr, unsigned int cnt)
{
    uint8_t* byte = ((uint8_t*)addr) + (cnt / 8);
    unsigned int bit = cnt % 8;

    *byte &= ~(1 << bit);

    return;
}

static void spiTransmit(const void* data, size_t len)
{
    /* Start transmit */
    if(HAL_SPI_Transmit_IT(&hspi2, (uint8_t*)data, len) != HAL_OK)
    {
        printf("SPI transmit failed\n");

        return;
    }

    /* Wait for transmit to complete */
    xSemaphoreTake(doneSem, portMAX_DELAY);

    return;
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    BaseType_t taskWoken;

    xSemaphoreGiveFromISR(doneSem, &taskWoken);

    portYIELD_FROM_ISR(taskWoken);

    return;
}

void nixieDriver_init(void)
{
    devMutex = xSemaphoreCreateMutex();
    doneSem = xSemaphoreCreateBinary();

    setDispEnable(true);

    return;
}

void nixieDriver_set(int* vals)
{
    int index;
    uint8_t bitmask[SHIFT_BYTES];
    int tensDigit;
    int onesDigit;

    xSemaphoreTake(devMutex, portMAX_DELAY);

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

    setLatchEnable(false);

    vTaskDelay(2);

    spiTransmit(bitmask, SHIFT_BYTES);

    vTaskDelay(2);

    setLatchEnable(true);

    vTaskDelay(2);

    xSemaphoreGive(devMutex);

    return;
}
