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

	// Screen events
#define CONFIG 	1
#define CONTROL 2
#define CONNECT 3

	/******************************************************************************/
	/*            										Functions            	  				            */
	/******************************************************************************/
	
	/**
	 * @brief  
	 * @param  data: 
	 * @retval 
	 */	
	void CamScreenP_Init(void (*Callback)(uint8_t screen));
	
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
	void CamScreenP_UpdateSpeed(int8_t speed);
	
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
	void CamScreenP_UpdateCableLenght(uint16_t lenght);
	
	/**
	 * @brief  
	 * @param  data: 
	 * @retval 
	 */	
	void CamScreenP_Errors(uint16_t flags);
	
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

	/**
	 * @brief  
	 * @param  data: 
	 * @retval 
	 */	
	int8_t CamScreenP_GetSpeed(void);

#endif
