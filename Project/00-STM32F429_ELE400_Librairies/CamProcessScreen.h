/**
 * @author  David Prairie-Bourgault and Alexi Demers
 * @email   alexidemers@gmail.com
 * @version v1
 * @ide     Keil uVision
 * @brief   This class controls the navigation between screen and the notifications
 *					to the main process when something happens that needs further processing.
 *					in summary, this library is the main process of the user interface's display.
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
	 * @brief  Initialize screen. buttons and event callback.
	 * @param  Callback: function that will be called when something happens on the screen
	 * @retval None
	 */	
	void CamScreenP_Init(void (*Callback)(uint8_t screen));
	
	/**
	 * @brief  Notifies the screen process that cable cam is communicating
	 * @param  state: true if cable cam communicates 
	 * @retval none
	 */	
	void CamScreenP_ControllerOnline(bool state);
	
	/**
	 * @brief  Updates actual speed on screen
	 * @param  speed: speed to show, in %
	 * @retval none
	 */	
	void CamScreenP_UpdateSpeed(int8_t speed);
	
	/**
	 * @brief  Updates cable cam battery level on screen
	 * @param  level: batt level, in %
	 * @retval none
	 */	
	void CamScreenP_UpdateBattLevel(uint8_t level);
	
	/**
	 * @brief  Updates cable cam position
	 * @param  pos: in decimeters
	 * @retval none
	 */	
	void CamScreenP_UpdatePosition(uint16_t pos);
	
	/**
	 * @brief  Updates acceleration value
	 * @param  accel: acceleration (value 1 to 5) 
	 * @retval none
	 */	
	void CamScreenP_UpdateAccel(uint8_t accel);
	
	/**
	 * @brief  Updates cable lenght config on screen
	 * @param  lenght: lenght to show, in dm 
	 * @retval none
	 */	
	void CamScreenP_UpdateCableLenght(uint16_t lenght);
	
	/**
	 * @brief  Show errors on screen
	 * @param  flags: errors flags (see defines)
	 * @retval none
	 */	
	void CamScreenP_Errors(uint16_t flags);
	
	/**
	 * @brief  Indicates the interface is connected or not to the cable cam
	 * @param  state: true if connected
	 * @retval none
	 */	
	void CamScreenP_SetConnected(bool state);
	
	/**
	 * @brief  Main process
	 * @param  none 
	 * @retval none 
	 */	
	void CamScreenP_RefreshScreen(void);
	
	/**
	 * @brief  Gets acceleration value set by user
	 * @param  none
	 * @retval uint8_t accel value
	 */	
	uint8_t CamScreenP_GetAccel(void);
	
	/**
	 * @brief  gets cable lenght set by user
	 * @param  none
	 * @retval uint8_t cable lenght
	 */	
	uint16_t CamScreenP_GetCableLenght(void);

	/**
	 * @brief  gets speed set by user
	 * @param  none
	 * @retval int8_t speed
	 */	
	int8_t CamScreenP_GetSpeed(void);

#endif
