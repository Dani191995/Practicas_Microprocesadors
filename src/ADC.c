#include "ADC.h"


void ADC_configuration()
{

	ADC_InitTypeDef ADC_struct;

	ADC_struct.ADC_ContinuousConvMode = DISABLE ;
	ADC_struct.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_struct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T4_TRGO ;
	ADC_struct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
	ADC_struct.ADC_NbrOfConversion = 1;
	ADC_struct.ADC_Resolution = ADC_Resolution_12b;
	ADC_struct.ADC_ScanConvMode = DISABLE ;

	ADC_Init(ADC1,&ADC_struct);

	/*
	  ADC_AnalogWatchdogSingleChannelConfig(ADC1, ADC_Channel_5);
	  ADC_AnalogWatchdogThresholdsConfig(ADC1, (2450*4095)/3300, (650*4095)/3300);
	  ADC_AnalogWatchdogCmd(ADC1, ADC_AnalogWatchdog_SingleRegEnable);

	  */

	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_192Cycles);

	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

	ADC_Cmd(ADC1, ENABLE);  //COMIENZA EL ADC

}





