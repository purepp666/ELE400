/**
 * @author  
 * @email   
 * @version v1
 * @ide     Keil uVision
 * @brief   
 */

#ifndef CAM_PROCESS_SCREEN_H
	#define CAM_PROCESS_SCREEN_H

	#include <stdbool.h>
	#include "stm32f4xx.h"
	#include "defines.h"
	#include "CamScreen.h"

	// possible errors
#define NO_ERROR 							0x0
#define CABLE_END 						0x1
#define BATT_TEMP 						0x2
#define BATT_LOW 							0x4
#define MOTOR_FORCES 					0x8
#define INVALID_COMMAND 			0x10
#define OBSTACLE 							0x20
#define EMERGENCY_STOP 				0x40
#define INTERFACES_CONFLICT 	0x80
#define TX_ERROR 							0x100
#define RX_ERROR 							0x200


	/******************************************************************************/
	/*            										Functions            	  				            */
	/******************************************************************************/
	
	/**
	 * @brief  
	 * @param  data: 
	 * @retval 
	 */	
	void CamScreenP_Init(void);
	
	/**
	 * @brief  
	 * @param  data: 
	 * @retval 
	 */	
	void CamScreenP_ControllerOnline(bool state);
	
	/**
	 * @brief  
	 * @param  data: 
	 * @retval 
	 */	
	void CamScreenP_UpdateSpeed(uint8_t speed);
	
	/**
	 * @brief  
	 * @param  data: 
	 * @retval 
	 */	
	void CamScreenP_UpdateBattLevel(uint8_t level);
	
	/**
	 * @brief  
	 * @param  data: 
	 * @retval 
	 */	
	void CamScreenP_UpdatePosition(uint16_t pos);
	
	/**
	 * @brief  
	 * @param  data: 
	 * @retval 
	 */	
	void CamScreenP_UpdateAccel(uint8_t accel);
	
	/**
	 * @brief  
	 * @param  data: 
	 * @retval 
	 */	
	void CamScreenP_UpdateCableLenght(uint16_t speed);
	
	/**
	 * @brief  
	 * @param  data: 
	 * @retval 
	 */	
	void CamScreenP_Errors(uint8_t flags);
	
	/**
	 * @brief  
	 * @param  data: 
	 * @retval 
	 */	
	void CamScreenP_SetConnected(bool state);
	
	/**
	 * @brief  
	 * @param  data: 
	 * @retval 
	 */	
	void CamScreenP_ReadButtons(uint8_t button_pressed);
	
	/**
	 * @brief  
	 * @param  data: 
	 * @retval 
	 */	
	void CamScreenP_RefreshScreen(void);
	
	/**
	 * @brief  
	 * @param  data: 
	 * @retval 
	 */	
	uint8_t CamScreenP_GetAccel(void);
	
	/**
	 * @brief  
	 * @param  data: 
	 * @retval 
	 */	
	uint16_t CamScreenP_GetCableLenght(void);


#endif
