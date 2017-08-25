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

#ifndef __GLOBALS_H
#define __GLOBALS_H

/* Includes *******************************************************************/
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/types.h>


/* Definitions ****************************************************************/

/* GPIO pin definitions */
#define DBG_LED_Pin             GPIO_PIN_0
#define DBG_LED_GPIO_Port       GPIOA

#define LED_RED_Pin             GPIO_PIN_1
#define LED_RED_GPIO_Port       GPIOA

#define LED_GRN_Pin             GPIO_PIN_2
#define LED_GRN_GPIO_Port       GPIOA

#define LED_BLU_Pin             GPIO_PIN_3
#define LED_BLU_GPIO_Port       GPIOA

#define PB1_Pin                 GPIO_PIN_6
#define PB1_GPIO_Port           GPIOA

#define PB2_Pin                 GPIO_PIN_5
#define PB2_GPIO_Port           GPIOA

#define PB3_Pin                 GPIO_PIN_4
#define PB3_GPIO_Port           GPIOA

#define PB4_Pin                 GPIO_PIN_6
#define PB4_GPIO_Port           GPIOB

#define GPS_RESET_Pin           GPIO_PIN_2
#define GPS_RESET_GPIO_Port     GPIOB

#define GPS_PPS_Pin             GPIO_PIN_13
#define GPS_PPS_GPIO_Port       GPIOC

#define GPS_RX_Pin              GPIO_PIN_10
#define GPS_RX_GPIO_Port        GPIOB

#define GPS_TX_Pin              GPIO_PIN_11
#define GPS_TX_GPIO_Port        GPIOB

#define DISP_LE_Pin             GPIO_PIN_14
#define DISP_LE_GPIO_Port       GPIOB

#define DISP_BL_Pin             GPIO_PIN_8
#define DISP_BL_GPIO_Port       GPIOA

#define DISP_CLK_Pin            GPIO_PIN_13
#define DISP_CLK_GPIO_Port      GPIOB

#define DISP_DATA_Pin           GPIO_PIN_15
#define DISP_DATA_GPIO_Port     GPIOB

#define USB_VBUS_Pin            GPIO_PIN_10
#define USB_VBUS_GPIO_Port      GPIOA

#define HV_EN_Pin               GPIO_PIN_9
#define HV_EN_GPIO_Port         GPIOB

/* Interrupt priorities - smaller number is higher priority.  Priorities must be
 * lower (larger) than configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY. */
#define EXTI15_10_IRQ_PRIORITY           5
#define SPI2_IRQ_PRIORITY                8
#define USART3_IRQ_PRIORITY              8
#define USB_LP_CAN_RX0_IRQ_PRIORITY      8
#define RTC_WKUP_IRQ_PRIORITY           10
#define SYSTICK_IRQ_PRIORITY            15

/* Task priorities - larger number is higher priority. */
#define GPS_TASK_PRIORITY                2
#define MAIN_TASK_PRIORITY               1

#endif /* __GLOBALS_H */
