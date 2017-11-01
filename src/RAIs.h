#include "stm32l1xx.h"
#include "stm32l_discovery_lcd.h"
#ifndef RAIS_H_
#define RAIS_H_

 bool inicio = TRUE;
uint16_t tiempo;
uint32_t capture;
void EXTI0_IRQHandler(void);
void TIM4_IRQHandler(void);
void DisplayTimeOnLCD(uint16_t tiempo);

#endif /* RAIS_H_ */
