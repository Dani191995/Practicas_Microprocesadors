#include "RAIs.h"


void ADC1_IRQHandler(void)
{
	float conv;
	if (ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET)
		{
			ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
			ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
			GPIO_ToggleBits(GPIOB,GPIO_Pin_6);
			conv = ADC_GetConversionValue(ADC1);
			DisplayConversionOnLCD(conv);
		}
	if (ADC_GetITStatus(ADC1, ADC_IT_AWD) != RESET)
	{
		ADC_ClearITPendingBit(ADC1, ADC_IT_AWD);
		ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
		LCD_GLASS_Clear();
		LCD_GLASS_DisplayString((uint8_t*)" ERROR");
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);
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
		 }

}

void EXTI0_IRQHandler (void){ //CUANDO PULSAMOS USER SALTA A LA SIGUIENTE INTERRUPCION

	if(EXTI_GetFlagStatus(EXTI_Line0)!=0){
		LCD_GLASS_DisplayString((uint8_t*)" START ");
		ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
		ADC_ITConfig(ADC1,ADC_IT_AWD,ENABLE);
		TIM_Cmd(TIM4, ENABLE);	//COMIENZA A CONTAR EL TIMER
		GPIO_WriteBit(GPIOB,GPIO_Pin_7, 1);
		EXTI_ClearITPendingBit(EXTI_Line0);	// LIMPIAMOS EL FLAG
	}
}

void DisplayConversionOnLCD(float conversion)	//FUNCION PARA MOSTRAR POR PANTALLA EL TIEMPO QUE HA TRANSCURRIDO CUANDO PULSAMOS EL BOTON 12
{
	char cad[]="";
	unsigned char entero;
	int decimal;

	conversion=(conversion*3)/4095;
	//conversion=conversion*1000;

	entero=conversion;
	decimal=(int)(conversion*1000)%1000;
	// convetimos los integer en char
		 sprintf(&cad,"%u%u",entero,decimal);
		 LCD_GLASS_Clear();
		 LCD_GLASS_DisplayString((uint8_t*) &cad);
	// mostramos por pantalla el tiempo
		 LCD_GLASS_WriteChar((uint8_t*) &cad[0], 1, FALSE, 1);

	}




