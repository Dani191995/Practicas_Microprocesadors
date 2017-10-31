#include "Configuration.h"

void Configuracion_Interrupcion()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);		//HABILITAMOS SYSCFG CLOCK
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);	//PARA TENER UNA EXTI EN EL PIN0(BOTON USER)

	//PARAMETROS DE CONFIGURACION
	EXTI_InitTypeDef Configuracion_EXTI;
	EXTI_StructInit(&Configuracion_EXTI);

	//PC0 SE CONECTA A EXTI_Line0
	Configuracion_EXTI.EXTI_Line= EXTI_Line0;
	Configuracion_EXTI.EXTI_Mode=EXTI_Mode_Interrupt;
	Configuracion_EXTI.EXTI_Trigger= EXTI_Trigger_Falling;
	Configuracion_EXTI.EXTI_LineCmd= ENABLE;
	EXTI_Init(&Configuracion_EXTI);

	//CONFIGURACION DE LA INTERRUPCION DEL TIMER
	Configuracion_EXTI.EXTI_Line= TIM4_IRQn;
	Configuracion_EXTI.EXTI_Mode=EXTI_Mode_Interrupt;
	Configuracion_EXTI.EXTI_Trigger= EXTI_Trigger_Rising;
	Configuracion_EXTI.EXTI_LineCmd= ENABLE;
	EXTI_Init(&Configuracion_EXTI);

	//HABILITAMOS LA INTERRUPCION Y DEFINIMOS LA INTERRUPCION GPIO_PIN_0 COMO BAJA PRIPORIDAD
	NVIC_InitTypeDef Configuracion_NVIC;
	Configuracion_NVIC.NVIC_IRQChannel=EXTI0_IRQn;
	Configuracion_NVIC.NVIC_IRQChannelCmd=ENABLE;
	Configuracion_NVIC.NVIC_IRQChannelPreemptionPriority= 0X0F;
	Configuracion_NVIC.NVIC_IRQChannelSubPriority=0x0f;


	NVIC_Init(&Configuracion_NVIC);

	Configuracion_NVIC.NVIC_IRQChannel=TIM4_IRQn;
	Configuracion_NVIC.NVIC_IRQChannelCmd=ENABLE;
	Configuracion_NVIC.NVIC_IRQChannelPreemptionPriority= 0X0E;
	Configuracion_NVIC.NVIC_IRQChannelSubPriority=0x0f;
	NVIC_Init(&Configuracion_NVIC);
}
