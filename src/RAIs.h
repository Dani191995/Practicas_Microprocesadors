#include "stm32l1xx.h"
#include "stm32l_discovery_lcd.h"
#ifndef RAIS_H_
#define RAIS_H_

void ADC1_IRQHandler(void);




 bool inicio = TRUE;
uint16_t conversion;
void EXTI0_IRQHandler(void);
void TIM4_IRQHandler(void);
void DisplayConversionOnLCD(uint16_t conversion);

#endif /* RAIS_H_ */
