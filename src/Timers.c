#include "Timers.h"


void RCC_Configuration()	//CONFIGURAMOS LOS DIFERENTES RELOJES
{
  /* Enable the GPIOs Clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC| RCC_AHBPeriph_GPIOD| RCC_AHBPeriph_GPIOE| RCC_AHBPeriph_GPIOH, ENABLE);

  /* Enable comparator clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_COMP | RCC_APB1Periph_LCD | RCC_APB1Periph_PWR,ENABLE);

  /* Enable SYSCFG */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG , ENABLE);

  /* Allow access to the RTC */
  PWR_RTCAccessCmd(ENABLE);

  /* Reset Backup Domain */
  RCC_RTCResetCmd(ENABLE);
  RCC_RTCResetCmd(DISABLE);

  /*!< LSE Enable */
  RCC_LSEConfig(RCC_LSE_ON);

  /*!< Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /*!< LCD Clock Source Selection */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
}

void Timer_Configuration()	//CONFIGURACION DEL TIMER 4
 {
 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
 TIM_TimeBaseStructure.TIM_Period = 62499;
 TIM_TimeBaseStructure.TIM_Prescaler = 63;
 TIM_TimeBaseStructure.TIM_ClockDivision = 0;
 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
 TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 TIM_SelectOutputTrigger( TIM4, TIM_TRGOSource_Update);



//trgo para el adc



 	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); //Update Event
 	TIM_ITConfig(TIM4, TIM_IT_CC1 , ENABLE); //IC1 asociado a PA12
 	TIM_ITConfig(TIM4, TIM_IT_CC2 , ENABLE); //OC2 asociado a PB7
 }

void Systick_Configuration()
{
  RCC_ClocksTypeDef RCC_Clocks;
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
}

