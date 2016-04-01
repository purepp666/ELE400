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
