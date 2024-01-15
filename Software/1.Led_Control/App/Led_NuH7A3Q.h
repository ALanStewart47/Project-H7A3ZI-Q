#ifndef __LED_NUH7A3Q_H
#define __LED_NUH7A3Q_H

#include "stm32h7xx_hal.h"

#include "main.h"

int create_random_number(void);
int Get_Random_LED_Port(void);
void Random_Delay_LED_Toggle(GPIO_TypeDef *GPIO,uint16_t pin);

#endif /* _LED_NUH7A3Q_H */
