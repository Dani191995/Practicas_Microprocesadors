//INCLUIMOS LAS LIBRERIAS QUE VAMOS A UTILIZAR---------
//#include "stm32l1xx.h"
#include "stm32l_discovery_lcd.h"
#include "stm32l1xx_tim.h"
#include "Configuration.h"
//------------------------------------------------------

// void Init_GPIOs (void);					//CONFIGURACION DE LOS PUERTOS DE ENTRADA Y SALIDA
//void RCC_Configuration(void);				//CONFIGURACION DEL RELOJ
 void Configuracion_Interrupcion(void);		//CONFIGURACION DE LA INTERRUPCION
 void Configuracion_Timer(void);

 static volatile uint32_t TimingDelay;
 RCC_ClocksTypeDef RCC_Clocks;

 void Config_Systick()						//CONFIGURACION DEL SYSTICK
 {
   RCC_GetClocksFreq(&RCC_Clocks);
   SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
 }


int main(void)
{
  RCC_Configuration();				//LLAMADA A LA FUNCION DE CONFIGURACION DEL RELOJ
  Init_GPIOs ();					//LLAMADA A LA FUNCION DE CONFIGURACION DE LOS PUERTOS DE ENTRADA/SALIDA
  Config_Systick();					//LLAMADA A LA FUNCION DE LA CONFIGURACION DEL SYSTCICK
  Configuracion_Interrupcion();		//LLAMADA A LA FUNCION DE LA CONFIGURACION DE LA INTERRUPCION
  Configuracion_Timer();

  LCD_GLASS_Init();					//INICIALIZAMOS LA PANTALLA
  LCD_GLASS_Clear();				//LIMPIAMOS LA PANTALLA
  LCD_GLASS_ScrollSentence((uint8_t*)" P2 TIMERS ",5,200);	//MOSTRAMOS UN SCROLL CON EL MESAJE INDICADO
  LCD_GLASS_Clear();
  LCD_GLASS_DisplayString((uint8_t*)" PULSE");				//MOSTRAMOS UN MENSAJE DE PULSE
  TIM_Cmd(TIM4, ENABLE);

  while(1){

  }	//BUCLE INFINITO A LA ESPERA DE LA INTERRUPCION

  return 0;
}

void EXTI0_IRQHandler(void)		//RUTINA DE ATENCION A LA INTERRUPCION O RAI
{
	if(EXTI_GetFlagStatus(EXTI_Line0)){
		  LCD_GLASS_Clear();
		  LCD_GLASS_DisplayString((uint8_t*)"	LIBEGTO ");
		  EXTI_ClearITPendingBit(EXTI_Line0);
	    }
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	{
		LCD_GLASS_DisplayString((uint8_t*)" fin ");
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}
}

void Delay(uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

void TimingDelay_Decrement(void)
{

  if (TimingDelay != 0x00)
  {
    TimingDelay--;
  }
}

