//INCLUIMOS LAS LIBRERIAS QUE VAMOS A UTILIZAR---------
#include "stm32l1xx.h"
#include "stm32l_discovery_lcd.h"
#include "stm32l1xx_tim.h"
//------------------------------------------------------

 void Init_GPIOs (void);					//CONFIGURACION DE LOS PUERTOS DE ENTRADA Y SALIDA
 void RCC_Configuration(void);				//CONFIGURACION DEL RELOJ
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

void RCC_Configuration(void)
{
  /* Enable the GPIOs Clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC| RCC_AHBPeriph_GPIOD| RCC_AHBPeriph_GPIOE| RCC_AHBPeriph_GPIOH, ENABLE);

  /* Enable comparator clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_COMP | RCC_APB1Periph_LCD | RCC_APB1Periph_PWR,ENABLE);

  /* Enable SYSCFG */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG , ENABLE);

  /* Allow access to the RTC */
  PWR_RTCAccessCmd(ENABLE);

  /* Reset Backup Domain */
  RCC_RTCResetCmd(ENABLE);
  RCC_RTCResetCmd(DISABLE);

  /*!< LSE Enable */
  RCC_LSEConfig(RCC_LSE_ON);

  /*!< Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}

  /*!< LCD Clock Source Selection */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
}

void  Init_GPIOs (void)
{
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC|RCC_AHBPeriph_GPIOD, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;

  //CONFIGURACION DEL BOTON USER-------------------
  GPIO_InitStructure.GPIO_Pin= GPIO_Pin_0;			//PIN 0 (BOTON USER)
  GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN;		//MODO ENTRADA
  GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_NOPULL;	//NO HACE FALTA RESISTENCIA(INTEGRADA)
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;	//VELOCIDAD
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  //-----------------------------------------------

 //CONFIGURACION DEL LED COMO SALIDA--------------------------
 GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
 GPIO_InitStructure.GPIO_Mode= GPIO_Mode_OUT;
 GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_NOPULL;
 GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;
 GPIO_Init(GPIOD, & GPIO_InitStructure);

  //-----------------------------------------------------------
//CONFIGURACION DE LA SALIDA POR LA PANTALA LCD

//PUERTO A
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_8 | GPIO_Pin_9 |GPIO_Pin_10 |GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init( GPIOA, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource15,GPIO_AF_LCD) ;

//PUERTO B
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9 \
                                 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init( GPIOB, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource12,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource14,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15,GPIO_AF_LCD) ;

//PUERTO C
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 \
                                 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_11 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init( GPIOC, &GPIO_InitStructure);


  GPIO_PinAFConfig(GPIOC, GPIO_PinSource0,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource1,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource2,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource3,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource8,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10,GPIO_AF_LCD) ;
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11,GPIO_AF_LCD) ;
}
void Configuracion_Interrupcion(void)
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

	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4, ENABLE);
}
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

