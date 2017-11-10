#include "GPIOs.h"

void  GPIOs_Configuration ()
{
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC|RCC_AHBPeriph_GPIOD, ENABLE);
  GPIO_InitTypeDef GPIO_InitStructure;

//CONFIGURACION DEL BOTON USER-------------------
  GPIO_InitStructure.GPIO_Pin= GPIO_Pin_0;			//PIN 0 (BOTON USER)
  GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IN;		//MODO ENTRADA
  GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_NOPULL;	//NO HACE FALTA RESISTENCIA(INTEGRADA)
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_2MHz;	//VELOCIDAD
  GPIO_Init(GPIOA, &GPIO_InitStructure);

//----------------------------------------------------------
//CONFIGURAMOS EL PIN A5 COMO ENTRADA analogica
     	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
     	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
     	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
     	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
     	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
     	GPIO_Init(GPIOA, &GPIO_InitStructure);
//----------------------------------------------------------

//CONFIGURACION DEL LED COMO SALIDA-------------------------------
    	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
    	 GPIO_Init(GPIOB, &GPIO_InitStructure);
//-------------------------------------------------------------------------------------------------------------------

 //CONFIGURACION DEL LED COMO SALIDA-------------------------------
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	 GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);
 //-------------------------------------------------------------------------------------------------------------------

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
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3,GPIO_AF_LCD) ;
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





