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
#include "cmsis_os.h"
#include "usb_device.h"

#include "debug.h"
#include "uart3.h"
#include "rtc.h"
#include "timezone.h"
#include "gps.h"
#include "nixieDriver.h"


/* Global variables **********************************************************/
SPI_HandleTypeDef hspi2;


/* Private variables **********************************************************/
static TIM_HandleTypeDef htim2;

static osThreadId defaultTaskHandle;

static const timeZoneRule_t timezoneDST =
    { "EDT", Second, Sun, Mar, 2, -4 * 60 };

static const timeZoneRule_t timezoneSTD =
    { "EST", First,  Sun, Nov, 2, -5 * 60 };


/* Private function prototypes ************************************************/
static void systemClockConfig(void);
static void gpioInit(void);
static void spi2Init(void);
static void tim2Init(void);

static void mainTask(void const * argument);

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
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /* Configure the Systick */
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, SYSTICK_IRQ_PRIORITY, 0);

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

    return;
}

static void spi2Init(void)
{
    hspi2.Instance               = SPI2;
    hspi2.Init.Mode              = SPI_MODE_MASTER;
    hspi2.Init.Direction         = SPI_DIRECTION_2LINES;
    hspi2.Init.DataSize          = SPI_DATASIZE_8BIT;
    hspi2.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    hspi2.Init.CLKPhase          = SPI_PHASE_2EDGE;
    hspi2.Init.NSS               = SPI_NSS_SOFT;
    hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    hspi2.Init.FirstBit          = SPI_FIRSTBIT_LSB;
    hspi2.Init.TIMode            = SPI_TIMODE_DISABLE;
    hspi2.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    hspi2.Init.CRCPolynomial     = 7;
    hspi2.Init.CRCLength         = SPI_CRC_LENGTH_DATASIZE;
    hspi2.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;
    HAL_SPI_Init(&hspi2);

    return;
}

static void tim2Init(void)
{
    TIM_MasterConfigTypeDef tim_master;
    TIM_OC_InitTypeDef      tim_oc;

    htim2.Instance               = TIM2;
    htim2.Init.Prescaler         = 0;
    htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim2.Init.Period            = 0xFFFF;
    htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_PWM_Init(&htim2);

    tim_master.MasterOutputTrigger = TIM_TRGO_RESET;
    tim_master.MasterSlaveMode     = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&htim2, &tim_master);

    tim_oc.OCMode     = TIM_OCMODE_PWM1;
    tim_oc.Pulse      = 0;
    tim_oc.OCPolarity = TIM_OCPOLARITY_HIGH;
    tim_oc.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim2, &tim_oc, TIM_CHANNEL_1);
    HAL_TIM_PWM_ConfigChannel(&htim2, &tim_oc, TIM_CHANNEL_2);
    HAL_TIM_PWM_ConfigChannel(&htim2, &tim_oc, TIM_CHANNEL_3);
    HAL_TIM_PWM_ConfigChannel(&htim2, &tim_oc, TIM_CHANNEL_4);

    HAL_TIM_MspPostInit(&htim2);

    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

    return;
}

static void mainTask(void const * argument)
{
    int       display[NUM_CNT];
    time_t    utcTime;
    struct tm utcTimeStruct = {0};
    time_t    localTime;
    struct tm localTimeStruct = {0};

    //MX_USB_DEVICE_Init();

    timezone_set(&timezoneDST, &timezoneSTD);

    while(true)
    {
        /* Determine current time */
        rtc_getTime(&utcTimeStruct, NULL);
        utcTime = mktime(&utcTimeStruct);

        localTime = timezone_toLocal(utcTime);
        localtime_r(&localTime, &localTimeStruct);

        /* Update nixie tube digits */
        display[0] = localTimeStruct.tm_hour;
        display[1] = localTimeStruct.tm_min;
        display[2] = localTimeStruct.tm_sec;
        nixieDriver_set(display);

        /* Wait for second to increment */
        rtc_wait();
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
    spi2Init();
    tim2Init();

    uart3_init();
    rtc_init();
    nixieDriver_init();
    gps_init();

    TIM2->CCR2 = 0x1FF;
    TIM2->CCR3 = 0x1FF;
    TIM2->CCR4 = 0x1FF;

    osThreadDef(mainTaskDef, mainTask, osPriorityNormal, 0, 128);
    defaultTaskHandle = osThreadCreate(osThread(mainTaskDef), NULL);
    debug_assert(defaultTaskHandle);

    /* Start scheduler */
    osKernelStart();

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
