#include "stm32l1xx.h"
#include "stm32l_discovery_lcd.h"
#ifndef RAIS_H_
#define RAIS_H_

 bool inicio = TRUE;

void EXTI0_IRQHandler(void);
void TIM4_IRQHandler(void);

#endif /* RAIS_H_ */
