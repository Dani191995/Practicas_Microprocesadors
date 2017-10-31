#include "RAIs.h"

void EXTI0_IRQHandler()		//RUTINA DE ATENCION A LA INTERRUPCION O RAI
{
	if(EXTI_GetFlagStatus(EXTI_Line0)){
		  LCD_GLASS_Clear();
		  LCD_GLASS_DisplayString((uint8_t*) " LIBEGTO ");
		  EXTI_ClearITPendingBit(EXTI_Line0);
	    }
}

void TIM4_IRQHandler()
{
	if (inicio == TRUE)
	 {
		inicio = FALSE;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	 }
	 /* TIM4_Time_Base */
	 if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	 {
		 LCD_GLASS_DisplayString((uint8_t*)" TROLL ");
		 TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

	 }
}


