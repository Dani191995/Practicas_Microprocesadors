#include "ADC.h"


void ADC_configuration(){

ADC_InitTypeDef ADC_struct;

ADC_struct.ADC_ContinuousConvMode = DISABLE ;
ADC_struct.ADC_DataAlign = ADC_DataAlign_Right;
ADC_struct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T4_TRGO ;
ADC_struct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
ADC_struct.ADC_NbrOfConversion = 1;
ADC_struct.ADC_Resolution = ADC_Resolution_12b;
ADC_struct.ADC_ScanConvMode = DISABLE ;

ADC_Init(ADC1,&ADC_struct);

}





