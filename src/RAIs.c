#include "RAIs.h"


void ADC1_IRQHandler(void)
{
	uint16_t conv;
	if (ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET)
		{
			ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
			ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
			conv = ADC_GetConversionValue(ADC1);
			DisplayConversionOnLCD(conv);
		}
	if (ADC_GetITStatus(ADC1, ADC_IT_AWD) != RESET)
	{
		ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
		ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
		GPIO_WriteBit(GPIOB, GPIO_Pin_7, DISABLE);
	}
}



void TIM4_IRQHandler()
{
	if (inicio == TRUE)	//LIMPIAMOS LOS FLAG DE TODAS LAS INTERRUPCIONES
	 {
		inicio = FALSE;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	 }

//CUANDO LLEGA AL FINAL DE LA CUENTA ENTRA EN AL INTERRUPCION
	 if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)	//EN EL CASO DE QUE LLEGUE A 15S SALTA LA SIGUIENTE INTERRUPCION
		 {
			 TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
			 GPIO_ToggleBits(GPIOB,GPIO_Pin_6);
		 }

}

void EXTI0_IRQHandler (void){ //CUANDO PULSAMOS USER SALTA A LA SIGUIENTE INTERRUPCION

	if(EXTI_GetFlagStatus(EXTI_Line0)!=0){
		LCD_GLASS_DisplayString((uint8_t*)" START ");
		TIM_Cmd(TIM4, ENABLE);	//COMIENZA A CONTAR EL TIMER
		ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
		GPIO_WriteBit(GPIOB,GPIO_Pin_7, 1);
		EXTI_ClearITPendingBit(EXTI_Line0);	// LIMPIAMOS EL FLAG
	}
}

void DisplayConversionOnLCD(uint16_t conversion)	//FUNCION PARA MOSTRAR POR PANTALLA EL TIEMPO QUE HA TRANSCURRIDO CUANDO PULSAMOS EL BOTON 12
{
	char cad[]="";
	sprintf(&cad, "%u", conversion);

	LCD_GLASS_Clear();
	//LCD_GLASS_DisplayString((uint8_t) cad);
	LCD_GLASS_WriteChar((uint8_t*) cad, 1, 0, 1);
	LCD_GLASS_WriteChar((uint8_t*) cad, 0, 0, 2);
	LCD_GLASS_WriteChar((uint8_t*) cad, 0, 0, 3);

	GPIO_ToggleBits(GPIOB,GPIO_Pin_6);
}


