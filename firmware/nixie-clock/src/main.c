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
#include "globals.h"
#include "stm32f3xx_hal.h"
#include "usb_device.h"

#include "FreeRTOS.h"
#include "task.h"

#include "debug.h"
#include "pwm.h"
#include "spi.h"
#include "uart.h"
#include "rtc.h"
#include "timezone.h"
#include "gps.h"
#include "nixieDriver.h"
#include "color.h"


/* Private variables **********************************************************/
static const timeZoneRule_t timezoneDST =
    { "EDT", Second, Sun, Mar, 2, -4 * 60 };

static const timeZoneRule_t timezoneSTD =
    { "EST", First,  Sun, Nov, 2, -5 * 60 };

static TaskHandle_t mainTaskHandle;
static TaskHandle_t ledTaskHandle;


/* Private function prototypes ************************************************/
static void systemClockConfig(void);
static void gpioInit(void);

static void mainTask(void * argument);
static void ledTask(void * argument);

extern void initialise_monitor_handles(void);
extern void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);


/* Private function definitions ***********************************************/
static void systemClockConfig(void)
{
    RCC_OscInitTypeDef       rcc_osc;
    RCC_ClkInitTypeDef       rcc_clk;
    RCC_PeriphCLKInitTypeDef periph_clk;

    /* Initializes the CPU, AHB and APB busses clocks */
    rcc_osc.OscillatorType = RCC_OSCILLATORTYPE_LSI |
                             RCC_OSCILLATORTYPE_HSE;
    rcc_osc.HSEState       = RCC_HSE_ON;
    rcc_osc.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    rcc_osc.HSIState       = RCC_HSI_ON;
    rcc_osc.LSIState       = RCC_LSI_ON;
    rcc_osc.PLL.PLLState   = RCC_PLL_ON;
    rcc_osc.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    rcc_osc.PLL.PLLMUL     = RCC_PLL_MUL9;
    HAL_RCC_OscConfig(&rcc_osc);

    /* Initializes the CPU, AHB and APB busses clocks */
    rcc_clk.ClockType      = RCC_CLOCKTYPE_HCLK   |
                             RCC_CLOCKTYPE_SYSCLK |
                             RCC_CLOCKTYPE_PCLK1  |
                             RCC_CLOCKTYPE_PCLK2;
    rcc_clk.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    rcc_clk.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    rcc_clk.APB1CLKDivider = RCC_HCLK_DIV4;
    rcc_clk.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&rcc_clk, FLASH_LATENCY_2);

    periph_clk.PeriphClockSelection = RCC_PERIPHCLK_USB |
                                      RCC_PERIPHCLK_RTC;
    periph_clk.RTCClockSelection    = RCC_RTCCLKSOURCE_LSE;
    periph_clk.USBClockSelection    = RCC_USBCLKSOURCE_PLL_DIV1_5;
    HAL_RCCEx_PeriphCLKConfig(&periph_clk);

    /* Configure the Systick interrupt time */
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/configTICK_RATE_HZ);

    /* Configure the Systick */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, SYSTICK_IRQ_PRIORITY, 0);

    debug_printf("SysClock: %u Hz\n", (unsigned)HAL_RCC_GetSysClockFreq());
    debug_printf("HCLK    : %u Hz\n", (unsigned)HAL_RCC_GetHCLKFreq());
    debug_printf("PCLK1   : %u Hz\n", (unsigned)HAL_RCC_GetPCLK1Freq());
    debug_printf("PCLK2   : %u Hz\n", (unsigned)HAL_RCC_GetPCLK2Freq());

    return;
}

static void gpioInit(void)
{
    GPIO_InitTypeDef gpio;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOF_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, GPS_RESET_Pin | DISP_LE_Pin | HV_EN_Pin, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(DISP_BL_GPIO_Port, DISP_BL_Pin, GPIO_PIN_SET);

    /*Configure GPIO pins */
    gpio.Pin  = PB3_Pin | PB2_Pin | PB1_Pin | USB_VBUS_Pin;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpio);

    gpio.Pin   = GPS_RESET_Pin | DISP_LE_Pin | HV_EN_Pin;
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &gpio);

    gpio.Pin   = DISP_BL_Pin;
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DISP_BL_GPIO_Port, &gpio);

    gpio.Pin  = PB4_Pin;
    gpio.Mode = GPIO_MODE_INPUT;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(PB4_GPIO_Port, &gpio);

    gpio.Pin  = GPS_PPS_Pin;
    gpio.Mode = GPIO_MODE_IT_RISING;
    gpio.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPS_PPS_GPIO_Port, &gpio);

    /* TIM2 pin configuration */
    gpio.Pin       = DBG_LED_Pin | LED_RED_Pin | LED_GRN_Pin | LED_BLU_Pin;
    gpio.Mode      = GPIO_MODE_AF_PP;
    gpio.Pull      = GPIO_NOPULL;
    gpio.Speed     = GPIO_SPEED_FREQ_LOW;
    gpio.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(DBG_LED_GPIO_Port, &gpio);

    /* SPI2 pin configuration */
    gpio.Pin       = DISP_CLK_Pin | DISP_DATA_Pin;
    gpio.Mode      = GPIO_MODE_AF_PP;
    gpio.Pull      = GPIO_NOPULL;
    gpio.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(DISP_CLK_GPIO_Port, &gpio);

    /* USART3 pin configuration */
    gpio.Pin       = GPS_RX_Pin | GPS_TX_Pin;
    gpio.Mode      = GPIO_MODE_AF_PP;
    gpio.Pull      = GPIO_PULLUP;
    gpio.Speed     = GPIO_SPEED_FREQ_HIGH;
    gpio.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPS_RX_GPIO_Port, &gpio);

    return;
}

void vApplicationIdleHook(void)
{
    /* Put CPU to sleep until next interrupt */
    __asm__("WFI");

    return;
}

static void mainTask(void * argument)
{
    int       display[NUM_CNT];
    time_t    utcTime;
    struct tm utcDateTime = {0};
    time_t    localTime;
    struct tm localDateTime = {0};

    //MX_USB_DEVICE_Init();

    timezone_set(&timezoneDST, &timezoneSTD);

    while(true)
    {
        /* Determine current time */
        rtc_getTime(&utcDateTime, NULL);
        utcTime = mktime(&utcDateTime);

        localTime = timezone_toLocal(utcTime);
        localtime_r(&localTime, &localDateTime);

        /* Update nixie tube digits */
        display[0] = localDateTime.tm_hour;
        display[1] = localDateTime.tm_min;
        display[2] = localDateTime.tm_sec;
        nixieDriver_set(display);

        /* Wait for second to increment */
        rtc_wait();
    }
}

static void ledTask(void * argument)
{
    hsv_t hsv = {0.0f, 1.0f, 0.08f};
    rgb_t rgb;

    while(true)
    {
        pwm_wait();

        hsv.hue += 0.1f;
        if(hsv.hue >= 360.0f)
        {
            hsv.hue -= 360.0f;
        }

        color_hsvToRgb(&hsv, &rgb);

        pwm_set(2, (uint32_t)(rgb.red * 65535.0f));
        pwm_set(3, (uint32_t)(rgb.grn * 65535.0f));
        pwm_set(4, (uint32_t)(rgb.blu * 65535.0f));
    }
}


/* Public function definitions ************************************************/
int main(void)
{
#if DEBUG
    initialise_monitor_handles();
#endif /* DEBUG */

    debug_printf("Program start\n");

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* Configure the system clock */
    systemClockConfig();

    /* Initialize all configured peripherals */
    gpioInit();

    pwm_init();
    spi_init();
    uart_init();
    rtc_init();
    nixieDriver_init();
    gps_init();

    pwm_set(2, 0x1FF);
    pwm_set(3, 0x1FF);
    pwm_set(4, 0x1FF);

    xTaskCreate(
        mainTask,
        "main",
        128,
        NULL,
        MAIN_TASK_PRIORITY,
        &mainTaskHandle);
    debug_assert(mainTaskHandle);

    xTaskCreate(
        ledTask,
        "led",
        128,
        NULL,
        LED_TASK_PRIORITY,
        &ledTaskHandle);
    debug_assert(ledTaskHandle);

    /* Start scheduler */
    vTaskStartScheduler();

    /* We should never get here as control is now taken by the scheduler */
    return -1;
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
    while(true);

    return;
}
#endif
