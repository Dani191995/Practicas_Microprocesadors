/* Includes */
#include <stddef.h>
#include "stm32l1xx.h"
#include "discover_board.h"
#include "stm32l_discovery_lcd.h"
/* Private typedef */
RCC_ClocksTypeDef RCC_Clocks;
GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
TIM_ICInitTypeDef TIM_ICInitStructure;
TIM_OCInitTypeDef TIM_OCInitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

/* Private function prototypes */
/* Private functions */
void Init_GPIOs (void);
void RCC_Configuration(void);
void TIM4_IRQHandler(void);
void Init_TIM4(void);

void Delay(uint32_t nTime);

void DisplayTimeOnLCD( uint16_t tiempo);
static volatile uint32_t TimingDelay;

RCC_ClocksTypeDef RCC_Clocks;
void Config_Systick()
{
	RCC_GetClocksFreq(&RCC_Clocks);
	SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
}

/**======================================================================
 ** Abstract: main program
 **=====================================================================*/
char inicio=1;
uint32_t Capture = 0;
uint16_t tiempo;
int main(void)
{
	RCC_Configuration();
	/* Init I/O ports */
	Init_GPIOs ();
	/* Init Systick */
	Config_Systick();
	/* Inicialización del LCD */
	LCD_GLASS_Init();
	LCD_GLASS_ScrollSentence((uint8_t*)"EJEMPLO DE BASE DE TIEMPO E IC",1,150);
	Delay(1000);
	LCD_GLASS_Clear();
	/* Switch on the leds at start */
	GPIO_LOW(LD_PORT,LD_GREEN);
	GPIO_LOW(LD_PORT,LD_BLUE);
	LCD_GLASS_DisplayString((uint8_t*)"ON");
	Delay(1000);
	/* Inicializamos TIM4 */
	Init_TIM4();
	while (1) // Infinite loop
	{ }
	return 0;
}
//=============================================================
void Init_TIM4(void)
{
	/* TIM4 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	/*------------------------NVIC Configuration -------------------------------*/
	/* Enable the TIM4 gloabal Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	//============================================================================
	// En system_stm32l1xx.c existe una configuración del system core clk de 16MHz
	//============================================================================
	/* Calculamos the prescaler value */
	uint16_t PrescalerValue = (uint16_t)(SystemCoreClock/1000)- 1;
	//Tick=16MHz/ Prescaler(16000)=1ms
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 20000; //20000 x 1ms = 20s
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue; //15999
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	/* TIM4 configuration: Input Capture mode Channel1 */
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

	/* Output Compare Toggle Mode configuration: Channel2 */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Toggle;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 15000; //1ms x 15000 = 15s
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Disable);

	//=============================================================
	/* TIM IT enable */
	//MUY Importante!!!!!!!!!!
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE); //Update Event
	TIM_ITConfig(TIM4, TIM_IT_CC1 , ENABLE); //IC1 asociado a PA12
	TIM_ITConfig(TIM4, TIM_IT_CC2 , ENABLE); //OC2 asociado a PB7
	//Habilitamos TMR4
	TIM_Cmd(TIM4, ENABLE);

}

//=============================================================
void TIM4_IRQHandler(void)
{
	/*if (inicio ==1)
	{
		inicio =0;
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
	}*/
	/* TIM4_Time_Base */
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		TIM_SetCompare2(TIM4, 15000);//programamos el OC TMR4-CH2
		tiempo =0;
	}
	/* TIM4_TIC_CH1 */
	if (TIM_GetITStatus(TIM4, TIM_IT_CC1) != RESET)
	{
		/* Clear TIM4 Capture compare interrupt pending bit */
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC1);
		/* Get the Input Capture value */
		tiempo = TIM_GetCapture1(TIM4); //IC -Capturamos el conteo
		//Convertimos a tiempo
		DisplayTimeOnLCD( tiempo);
	}
	/* TIM4_TOC_CH2 */
	if (TIM_GetITStatus(TIM4, TIM_IT_CC2) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);
		GPIO_WriteBit(LD_PORT,LD_GREEN, (BitAction)(1 - GPIO_ReadOutputDataBit(LD_PORT,LD_GREEN)));
		Capture = TIM_GetCapture2(TIM4);
		TIM_SetCompare2(TIM4, Capture + 500);
	}
}

//=============================================================
int s;
void DisplayTimeOnLCD( uint16_t tiempo)
{
	char decenas, unidades, decimales;
	decenas=tiempo/10000;
	unidades=tiempo/1000;
	decimales=tiempo/100;
	LCD_GLASS_WriteChar((uint8_t*)decenas, FALSE, FALSE, 0);
	LCD_GLASS_WriteChar((uint8_t*)unidades, FALSE, FALSE, 1);
	LCD_GLASS_WriteChar((uint8_t*)decimales, TRUE, TRUE, 2);
	/*char cad[]="";
	sprintf(&cad,"%d",tiempo);
	s=sizeof(cad);
	LCD_GLASS_Clear();
	LCD_GLASS_DisplayString((uint8_t*) cad);
	LCD_GLASS_WriteChar((uint8_t*)cad, TRUE, TRUE,s-3);*/
}

//=============================================================
void RCC_Configuration(void)
{
	/* Enable the GPIOs Clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC|
			RCC_AHBPeriph_GPIOD| RCC_AHBPeriph_GPIOE| RCC_AHBPeriph_GPIOH, ENABLE);

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
//=============================================================
void Init_GPIOs (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure PA12 (Button) como input */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* GPIOB Configuration: Pin 6 and 7 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_TIM4);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_TIM4);


	//===============================================================================
	/*El RI consta de 3 etapas. 1-Definir puerto 2- Conectarlo al RI 3-Conectar RI al Timer*/
	/*Debemos conectar PA12 al RI y éste a su vez al TMR4 CH1*/
	/* Redirigimos PA12 al Routing Interface */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_RI);

	/* Seleccionamos TIM4 dentro del RI*/
	SYSCFG_RITIMSelect(TIM_Select_TIM4);

	/* Input Capture CH1 se mapea hacia RI */
	/*OJO hay que buscar en el manual/153 y comprobar que se pueda routear al ch1*/
	SYSCFG_RITIMInputCaptureConfig(RI_InputCapture_IC1, RI_InputCaptureRouting_3);

	/*usamos AF para el PB7 (LED verde) que puede ser conectado al CH2 del TIM4*/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7,GPIO_AF_TIM4);

	// ==============================================================================
	/* Configure Output for LCD */
	/* Port A */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_8 | GPIO_Pin_9
			|GPIO_Pin_10 |GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1,GPIO_AF_LCD) ;
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2,GPIO_AF_LCD) ;
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3,GPIO_AF_LCD) ;
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource8,GPIO_AF_LCD) ;
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9,GPIO_AF_LCD) ;
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10,GPIO_AF_LCD) ;
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource15,GPIO_AF_LCD) ;
	/* Configure Output for LCD */
	/* Port B */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9 \
			| GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 |
			GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init( GPIOB, &GPIO_InitStructure);
#ifdef USE_ALL_LCD_SEGMENTS
	/**
	 * Note!
	 * PB3 is connected to C, M, COLON, and DP segments for the second digit on the LCD
	 * PB3 is also the SWO pin used for the Serial Wire Viewer (SWV)
	 * If PB3 is used by LCD then SWV will not work for the STM32L_DISCOVERY board
	 **/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3,GPIO_AF_LCD) ;
#endif
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
	/* Configure Output for LCD */
	/* Port C*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 \
			| GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_11
			;
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
