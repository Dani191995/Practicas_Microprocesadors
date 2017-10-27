STM32L-Discovery board

Note:
Version 1.0.0 of <system_stm32l1xx.c> is used to receive a 16 MHz System Core 
Clock based on HSI. 

The GPIO pin PB3 is connected to C, M, COLON, and DP segments for 
the second digit on the STM32L-Discovery LCD. However, PB3 is also 
the SWO pin used for the Serial Wire Viewer (SWV). 

A define "USE_ALL_LCD_SEGMENTS" needs to be set in <main.c> if you 
want all segments to work on the LCD. 

Normally, when the define "USE_ALL_LCD_SEGMENTS" not is set, PB3 
functions as SWO pin so that SWV shall work. 
