# MIT License
#
# Copyright (c) 2017 Jeff Kubascik
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

PROJECT(nixie-clock)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
ENABLE_LANGUAGE(ASM)

FIND_PACKAGE(CMSIS REQUIRED)
FIND_PACKAGE(STM32HAL COMPONENTS gpio dma REQUIRED)

SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DDEBUG")
SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -DRELEASE")

SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-wchar-size-warning --specs=nano.specs --specs=rdimon.specs -lrdimon")

ADD_SUBDIRECTORY(Middlewares/Third_Party/FreeRTOS)
ADD_SUBDIRECTORY(Middlewares/ST/STM32_USB_Device_Library)

INCLUDE_DIRECTORIES(
    Inc
    ${CMSIS_INCLUDE_DIRS}
    ${STM32HAL_INCLUDE_DIR}
    ${FREERTOS_INCLUDE_DIRS}
    ${STM32_USB_DEV_INCLUDE_DIRS}
)

SET(PROJECT_SOURCES
    Src/gps.c
    Src/main.c
    Src/nixieDriver.c
    Src/pwm.c
    Src/rtc.c
    Src/spi.c
    Src/stm32f3xx_hal_msp.c
    Src/stm32f3xx_it.c
    Src/timezone.c
    Src/uart3.c
#    Src/usb_device.c
#    Src/usbd_cdc_if.c
#    Src/usbd_conf.c
#    Src/usbd_desc.c
#    Src/newlib.c
)

SET(STM32_LINKER_SCRIPT STM32F302C8Tx_FLASH.ld)

ADD_EXECUTABLE(${CMAKE_PROJECT_NAME}
	${PROJECT_SOURCES}
	${CMSIS_SOURCES}
	${STM32HAL_SOURCES}
	${FREERTOS_SOURCES}
	${STM32_USB_DEV_PROJECT_SOURCES}
)

STM32_SET_TARGET_PROPERTIES(${CMAKE_PROJECT_NAME})
STM32_ADD_HEX_BIN_TARGETS(${CMAKE_PROJECT_NAME})
STM32_PRINT_SIZE_OF_TARGETS(${CMAKE_PROJECT_NAME})
