/**
 * @author  Alexi Demers
 * @email   alexidemers@gmail.com
 * @version v1
 * @ide     Keil uVision
 * @brief   CamUart_unication frames library for STM32F429. This library handles
 *					sending messages and receiving messages from the embedded controller
 *					of the cable cam. The wireless CamUart_unication module used is 
 *					Xbee.
 */
 
#ifndef ELE400_CamUart_UNICATION_FRAME_H
	#define ELE400_CamUart_UNICATION_FRAME_H
	
#include <stdbool.h>
#include "stm32f4xx.h"
#include "defines.h"

	/******************************************************************************/
	/*            										Defines	            	  				            */
	/******************************************************************************/ 

	/**
	 * @brief control frame informations
	 */
	typedef struct {
		int8_t speed;
		bool emergency_stop;
	}ControlCommand;
	/**
	 * @brief configuration frame informations
	 */
	typedef struct {
		uint8_t accel;
		uint16_t cable_lenght;
	}ConfigCommand;
	/**
	 * @brief data contained in a received message
	 */
	typedef struct {
		uint8_t battery_level;
		int16_t position;
		int8_t aimed_speed;
		int8_t actual_speed;
		uint8_t accel;
		uint16_t cable_lenght;
		uint8_t connected_interface_address;
		uint8_t error;
	}CamRxData;

	/******************************************************************************/
	/*            										Functions            	  				            */
	/******************************************************************************/ 
	
	/**
	 * @brief  This function initialize everything needed to CamUart_unicate to the XBee module
	 * @param  this_device_address: address of your device
	 * @retval None
	 */
		void CamUart_Init(uint8_t this_device_address);
		
	/**
	 * @brief  Sends a frame that indicates to the receiver that a new interface tries to connect
	 * @param  this_device_address: address of your device
	 * @retval None
	 */	
		void CamUart_SendConnectFrame(void);
		
	/**
	 * @brief Sends a frame that indicates to the receiver that the interface will stop sending CamUart_ands
	 * @param none
	 * @retval none
	 */
		void CamUart_SendDisconnectFrame(void);
		
	/**
	 * @brief  Sends a configuration frame for the cable camera controller
	 * @param  data: data to send
	 * @retval None
	 */	
		void CamUart_SendConfigFrame(const ConfigCommand data);	
	
	/**
	 * @brief  Sends a control frame to the calbe cam controller. see ControlCamUart_and struct
	 * @param  data: data to send
	 * @retval None
	 */	
		void CamUart_SendControlFrame(const ControlCommand data);	
		
	/**
	 * @brief  returns the number of rx errors encountered since last call of this function
	 * @param  data: data to send
	 * @retval uint8_t rx_errors
	 */
		uint8_t CamUart_GetRxErrorsCount(void);
		
	/**
	 * @brief pops a message from the rx buffer.
	 * @param  data the data read
	 * @retval bool true if a message has been read
	 */	
		bool CamUart_ReadMessage(CamRxData *data);

#endif
