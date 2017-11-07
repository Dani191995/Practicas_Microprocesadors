//USAMOS EL BOTON USER PARA COMENZAR LA CUENTA DEL TIMER Y EL BOTON PA12 PARA MOSTRAR
//EL TIEMPO TRANSCURRIDO, EL LED PB7 SE ENCIENDE A LOS 10 SEGUNDOS A MODO DE AVISO CADA
//0,5 SEGUNDOS PARPADEA.
//PARA PODER REACTIVAR EL TIMER ES NECESARIO PULSAR EL BOTON USER DE NUEVO.
// HEMOS DIVIDIDO EL CODIGO EN DIFERENTES ARCHIVOS.
//INCLUIMOS LAS LIBRERIAS QUE VAMOS A UTILIZAR---------
#include "GPIOs.h"
#include "Timers.h"
#include "Interruptions.h"
#include "ADC.h"
//------------------------------------------------------

 static volatile uint32_t TimingDelay;


int main(void)
{
  RCC_Configuration();				//LLAMADA A LA FUNCION DE CONFIGURACION DEL RELOJ
  GPIOs_Configuration ();			//LLAMADA A LA FUNCION DE CONFIGURACION DE LOS PUERTOS DE ENTRADA/SALIDA
  ADC_Configuration();
  Systick_Configuration();			//LLAMADA A LA FUNCION DE LA CONFIGURACION DEL SYSTCICK
  Interruption_Configuration();		//LLAMADA A LA FUNCION DE LA CONFIGURACION DE LA INTERRUPCION


  LCD_GLASS_Init();					//INICIALIZAMOS LA PANTALLA
  LCD_GLASS_Clear();				//LIMPIAMOS LA PANTALLA
  LCD_GLASS_ScrollSentence((uint8_t*)" ** P3 ADC ** ",1,200);	//MOSTRAMOS UN SCROLL CON EL MESAJE INDICADO
  LCD_GLASS_Clear();
  LCD_GLASS_DisplayString((uint8_t*)" PULSE");				//MOSTRAMOS UN MENSAJE DE PULSE

  Timer_Configuration();

  while(1){

  }	//BUCLE INFINITO A LA ESPERA DE LA INTERRUPCION

  return 0;
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

