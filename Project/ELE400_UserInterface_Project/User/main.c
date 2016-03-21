/**
 *	Keil project for USART using strings
 *
 *  Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */

#include "stm32f4xx.h"

#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "../00-STM32F429_ELE400_Librairies/ele400_communication_frames.h"

/******************************************************************************/
/*            									Global Variables           	      			 		  */
/******************************************************************************/ 

uint32_t timer_counter_ = 0;
uint32_t counter_compare_ = 0;

/******************************************************************************/
/*            									      Main           	      			 		  			*/
/******************************************************************************/ 

int main(void) {
	
	SystemInit();
	
	InitComms(0x0A);
	

	while(1){
		
	}
	
	/*char buffer[100];
	
	SystemInit();

	TM_USART_Init(USART1, TM_USART_PinsPack_2, 115200);
	
	TM_USART_Puts(USART1, "Hello world\n");

	while (1) {
		if (TM_USART_Gets(USART1, buffer, sizeof(buffer))) {
			TM_USART_Puts(USART1, buffer);
		}
	}*/
}

/******************************************************************************/
/*            							Functions definitions           	     		  			*/
/******************************************************************************/ 

void TM_DELAY_1msHandler(void){
	timer_counter_++;
}

