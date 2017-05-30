#ifndef __NIXIE_DRIVER_H
#define __NIXIE_DRIVER_H

#include "stm32f3xx_hal.h"
#include "stm32f3xx_hal_spi.h"

#define NUM_CNT 3

void nixieDriver_init(void);
void nixieDriver_set(int* vals);

#endif /* __NIXIE_DRIVER_H */
