#ifndef __LED_NUH7A3Q_H
#define __LED_NUH7A3Q_H

#include "stm32h7xx_hal.h"

#include "main.h"


void LED_Toggle(GPIO_TypeDef *GPIO,uint16_t pin,uint32_t delay);
void Random_LED_Toggle(GPIO_TypeDef *GPIO,uint16_t pin);

#endif /* _LED_NUH7A3Q_H */