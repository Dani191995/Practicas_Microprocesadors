#include "RAIs.h"
void TIM4_IRQHandler()
{
	if (inicio == TRUE)	//LIMPIAMOS LOS FLAG DE TODAS LAS INTERRUPCIONES
	 {
		inicio = FALSE;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1 );
		TIM_ClearITPendingBit( TIM4, TIM_IT_CC2);
	 }

//CUANDO LLEGA AL FINAL DE LA CUENTA ENTRA EN AL INTERRUPCION
	 if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)	//EN EL CASO DE QUE LLEGUE A 15S SALTA LA SIGUIENTE INTERRUPCION
		 {
			 LCD_GLASS_DisplayString((uint8_t*)"FIN T");
			 TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
			 TIM_SetCompare2(TIM4, 10000);	//se reconfigura el pulso a 10s, porque se ha cambiado anteriormente
			 TIM_Cmd(TIM4, DISABLE);		//desactivamos el timer, a la espera de que se pulse PA0
			 tiempo=0;
		 }

/* TIM4_TIC_CH1 */
	 	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)	//EN EL CASO DE PULSAR EL BOTON 12 CAPTURAMOS EL TIEMPO Y LO MOSTRAMOS POR PANTALLA
	 	{
	 	/* Clear TIM4 Capture compare interrupt pending bit */
	 	TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
	 	/* Get the Input Capture value */
	 	tiempo = TIM_GetCapture1(TIM4); //IC -Capturamos el conteo
	 	//Convertimos a tiempo
	 	DisplayTimeOnLCD(tiempo);
	 	}

/* TIM4_TOC_CH2 */
		if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)	//ESTE CANAL ESTA ASOCIADO AL LED 7 Y SALTA CUANDO LLEGA A 10S(SE PUEDE CAMBIAR EN TIMERS.C)
		 	 {
		 	 TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
		 /* ponemos el PB7(LED) en el estado contrario al que estaba antes de entrar en la interrupcion */
		 	 GPIO_WriteBit(GPIOB,GPIO_Pin_7, (BitAction)(1 - GPIO_ReadOutputDataBit(GPIOB,GPIO_Pin_7)));
		 	 capture = TIM_GetCapture2(TIM4);
		 /* despues de capturar el tiempo en el que se hace el pulso, lo aumentamos de 0.5s para que se vuelva a repetir y haga "parpadear" al led */
		 	 TIM_SetCompare2(TIM4, capture + 500);

		 	 }
}
void EXTI0_IRQHandler (void){ //CUANDO PULSAMOS USER SALTA A LA SIGUIENTE INTERRUPCION

	if(EXTI_GetFlagStatus(EXTI_Line0)!=0){
		LCD_GLASS_DisplayString((uint8_t*)" START ");
		TIM_Cmd(TIM4, ENABLE);	//COMIENZA A CONTAR EL TIMER
		EXTI_ClearITPendingBit(EXTI_Line0);	// LIMPIAMOS EL FLAGç
		GPIO_WriteBit(GPIOB,GPIO_Pin_7, 0);

	}
}

void DisplayTimeOnLCD(uint16_t tiempo)	//FUNCION PARA MOSTRAR POR PANTALLA EL TIEMPO QUE HA TRANSCURRIDO CUANDO PULSAMOS EL BOTON 12
{
	char cad[]="";
/* dividiremos el tiempo capturado para señalizar especificamente lo que queramos */
	uint8_t decena,unidad,decimal1,decimal2,decimal3;
	decena=tiempo/10000;
	unidad=(tiempo%10000)/1000;
	decimal1=(tiempo%1000)/100;
	decimal2=(tiempo%100)/10;
	decimal3=tiempo%10;
/* convetimos los integer en char */
	 sprintf(&cad[0],"%d",decena);
	 sprintf(&cad[1],"%d",unidad);
	 sprintf(&cad[2],"%d",decimal1);
	 sprintf(&cad[3],"%d",decimal2);
	 sprintf(&cad[4],"%d",decimal3);
	 LCD_GLASS_Clear();
/* mostramos por pantalla el tiempo */
	 LCD_GLASS_WriteChar((uint8_t*) &cad[0], FALSE, FALSE, 1);
	 LCD_GLASS_WriteChar((uint8_t*) &cad[1], TRUE, TRUE, 2);
	 LCD_GLASS_WriteChar((uint8_t*) &cad[2], FALSE, FALSE, 3);
	 LCD_GLASS_WriteChar((uint8_t*) &cad[3], FALSE, FALSE, 4);
	 LCD_GLASS_WriteChar((uint8_t*) &cad[4], FALSE, FALSE, 5);
}
