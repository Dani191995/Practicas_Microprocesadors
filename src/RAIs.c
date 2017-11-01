#include "RAIs.h"
void TIM4_IRQHandler()
{
	if (inicio == TRUE)
	 {
		inicio = FALSE;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 );
		TIM_ClearITPendingBit( TIM4, TIM_IT_CC2);
	 }
//CUANDO LLEGA AL FINAL DE LA CUENTA ENTRA EN AL INTERRUPCION
	 if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
		 {
			 LCD_GLASS_DisplayString((uint8_t*)"FIN T");
			 TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
			 TIM_SetCompare2(TIM4, 15000);
			 TIM_Cmd(TIM4, DISABLE);
			 tiempo=0;
		 }
	 /* TIM4_TIC_CH1 */
	 	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	 	{
	 			/* Clear TIM4 Capture compare interrupt pending bit */
	 				 		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
	 				 		/* Get the Input Capture value */
	 				 		tiempo = TIM_GetCapture1(TIM4); //IC -Capturamos el conteo
	 				 		//Convertimos a tiempo
	 				 		DisplayTimeOnLCD(tiempo);
	 	}
	 	/* TIM4_TOC_CH2 */
	 	if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)
	 	{
	 		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
	 		GPIO_WriteBit(GPIOB,GPIO_Pin_7, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_7)));
	 		capture = TIM_GetCapture2(TIM4);
	 		TIM_SetCompare2(TIM4, capture + 500);//añadimos 0.5s mas para que haga el parpadeo


	 	}
}
void EXTI0_IRQHandler (void){                  // Rutina de Atencion a la Interrupcion

	if(EXTI_GetFlagStatus(EXTI_Line0)!=0){
	LCD_GLASS_DisplayString((uint8_t*)" START ");
	TIM_Cmd(TIM4, ENABLE);
	EXTI_ClearITPendingBit(EXTI_Line0);                // Limpiamos el flag

	}
}

void DisplayTimeOnLCD(uint16_t tiempo)
{
	char cad[]="";
	uint8_t decena,unidad,decimal1,decimal2,decimal3;
	decena=tiempo/10000;
	unidad=(tiempo%10000)/1000;
	decimal1=(tiempo%1000)/100;
	decimal2=(tiempo%100)/10;
	decimal3=tiempo%10;

	 sprintf(&cad[0],"%d",decena);
	 sprintf(&cad[1],"%d",unidad);
	 sprintf(&cad[2],"%d",decimal1);
	 sprintf(&cad[3],"%d",decimal2);
	 sprintf(&cad[4],"%d",decimal3);
	 LCD_GLASS_Clear();
	 LCD_GLASS_WriteChar((uint8_t*) &cad[0], FALSE, FALSE, 1);
	 LCD_GLASS_WriteChar((uint8_t*) &cad[1], TRUE, TRUE, 2);
	 LCD_GLASS_WriteChar((uint8_t*) &cad[2], FALSE, FALSE, 3);
	 LCD_GLASS_WriteChar((uint8_t*) &cad[3], FALSE, FALSE, 4);
	 LCD_GLASS_WriteChar((uint8_t*) &cad[4], FALSE, FALSE, 5);
}
