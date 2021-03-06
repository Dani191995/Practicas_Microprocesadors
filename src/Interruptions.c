#include "Interruptions.h"

void Interruption_Configuration()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);		//HABILITAMOS SYSCFG CLOCK
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);			//ACTIVAMOS EL RELOJ DEL TIMER 4

	//PARAMETROS DE CONFIGURACION
	EXTI_InitTypeDef Configuracion_EXTI;
	EXTI_StructInit(&Configuracion_EXTI);

//CONFIGURACION DE LA INTERRUPCION DEL TIMER-------------------
	 NVIC_InitTypeDef NVIC_InitStructure;
	 NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);
//---------------------------------------------------------------
//CONFIGURAMOS LA INTERRUPCION DEL BOTON USER---------------------
	 EXTI_InitTypeDef struct_EXTI;
	 	 struct_EXTI.EXTI_Line = EXTI_Line0;
	 	 struct_EXTI.EXTI_Mode = EXTI_Mode_Interrupt ;
	 	 struct_EXTI.EXTI_Trigger = EXTI_Trigger_Falling;
	 	 struct_EXTI.EXTI_LineCmd = ENABLE;
	 	 EXTI_Init(&struct_EXTI);
//------------------------------------------------------------------
//CONFIGURAMOS LA PRIORIDAD DEL NVIC SOBRE EL PULSADOR USER----------
	 NVIC_InitTypeDef struct_NVIC;
	 struct_NVIC.NVIC_IRQChannel = EXTI0_IRQn;
	 struct_NVIC.NVIC_IRQChannelCmd = ENABLE ;
	 struct_NVIC.NVIC_IRQChannelPreemptionPriority = 0x0F ;
	 struct_NVIC.NVIC_IRQChannelSubPriority = 0x0F;
	 NVIC_Init(&struct_NVIC);
	 SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);  // Configuracion para tener una exti en PA0
//----------------------------------------------------------------
}




