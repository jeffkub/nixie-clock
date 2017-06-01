#include "nixieDriver.h"

#include <stdbool.h>

#include "FreeRTOS.h"
#include "portmacro.h"
#include "task.h"
#include "semphr.h"

#define TUBE_CNT     6
#define DIGIT_CNT   10

#define SHIFT_BYTES  8

extern SPI_HandleTypeDef hspi2;

static const uint8_t m_nixieBitmap[TUBE_CNT][DIGIT_CNT] =
{
    { 22, 23, 24, 25,  6,  7,  8,  9, 10, 21 },
    { 17, 18, 19, 20, 11, 12, 13, 14, 15, 16 },

    { 30, 31, 32,  1,  2,  3,  4,  5, 28, 29 },
    { 36, 35, 34, 33, 60, 61, 62, 63, 64, 37 },

    { 45, 46, 47, 48, 49, 50, 51, 52, 43, 44 },
    { 40, 41, 42, 53, 54, 55, 56, 57, 38, 39 }
};

static SemaphoreHandle_t m_devMutex;
static SemaphoreHandle_t m_doneSem;

static void setDispEnable(bool state);
static void setLatchEnable(bool state);
static void setbit(void* addr, unsigned int cnt);
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

static void setbit(void* addr, unsigned int cnt)
{
    uint8_t* byte = ((uint8_t*)addr) + (cnt / 8);
    unsigned int bit = cnt % 8;

    *byte |= 1 << bit;

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
    xSemaphoreTake(m_doneSem, portMAX_DELAY);

    return;
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    BaseType_t taskWoken;

    xSemaphoreGiveFromISR(m_doneSem, &taskWoken);

    portYIELD_FROM_ISR(taskWoken);

    return;
}

void nixieDriver_init(void)
{
    m_devMutex = xSemaphoreCreateMutex();
    m_doneSem = xSemaphoreCreateBinary();

    setDispEnable(true);

    return;
}

void nixieDriver_set(int* vals)
{
    int index;
    uint8_t bitmask[SHIFT_BYTES] = {0};
    int tensDigit;
    int onesDigit;

    xSemaphoreTake(m_devMutex, portMAX_DELAY);

    for(index = 0; index < NUM_CNT; index++)
    {
        tensDigit = vals[index] / DIGIT_CNT;
        onesDigit = vals[index] % DIGIT_CNT;

        if(tensDigit >= DIGIT_CNT)
        {
            tensDigit = DIGIT_CNT-1;
        }

        setbit(bitmask, m_nixieBitmap[index*2][tensDigit] - 1);
        setbit(bitmask, m_nixieBitmap[index*2 + 1][onesDigit] - 1);
    }

    setLatchEnable(false);

    vTaskDelay(2);

    spiTransmit(bitmask, SHIFT_BYTES);

    vTaskDelay(2);

    setLatchEnable(true);

    vTaskDelay(2);

    xSemaphoreGive(m_devMutex);

    return;
}
