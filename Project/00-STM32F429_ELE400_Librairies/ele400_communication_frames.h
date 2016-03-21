/**
 * @author  Alexi Dmeers
 * @email   alexidemers@gmail.com
 * @version v1
 * @ide     Keil uVision
 * @brief   Communication frames library for STM32F429. This library handles
 *					sending messages and receiving messages from the embedded controller
 *					of the cable cam. The wireless communication module used is 
 *					Xbee.
 */
 
#include "stm32f4xx.h"
#include "defines.h"


#ifndef ELE400_COMMUNICATION_FRAME_H
	#define ELE400_COMMUNICATION_FRAME_H

	/**
	 * This function initialize everything needed to communicate to the XBee module
	 */
	void InitComms();



#endif