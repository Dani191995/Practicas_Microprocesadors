#include "Configuration.h"

void Configuracion_Timer(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseInitTypeDef Timer_estructura;
	Timer_estructura.TIM_ClockDivision= 0;
	Timer_estructura.TIM_CounterMode=TIM_CounterMode_Up ;
	Timer_estructura.TIM_Period=63999;
	Timer_estructura.TIM_Prescaler= 3749;
	TIM_TimeBaseInit(TIM4,&Timer_estructura);
}
