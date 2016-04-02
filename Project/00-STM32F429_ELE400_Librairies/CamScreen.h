/**
 * @author  
 * @email   
 * @version v1
 * @ide     Keil uVision
 * @brief   
 */

#ifndef ECRAN_IMAGE_H
	#define ECRAN_IMAGE_H

#include <stdio.h>
#include "defines.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_stmpe811.h"
#include "tm_stm32f4_ili9341_button.h"
#include "tm_stm32f4_disco.h"

#define NOMBREERREUR 11 // nombre de différentes erreurs possibles.


/******************************************************************************/
/*            										Defines	            	  				            */
/******************************************************************************/
	/**
	 * @brief 
	 */
	typedef struct {
		uint8_t Accelation;      
		uint16_t LenghtCable;      
	} T_Config_Setting;

	/**
	 * @brief 
	 */
	typedef enum {
		Controller_Offline,
		Controller_Online
	} T_ControllerStatus;


	/**
	 * @brief 
	 */
	typedef struct {
		T_ControllerStatus ControllerStatus; //0->Offline 1->Online
		uint8_t BattLevel;
		int8_t VitesseVoulu;
		int8_t VitesseReel;
		uint16_t Position;
		uint8_t errors_flags;
	} T_Controle_Information;
	
	/**
	 * @brief buttons pressed flags
	 */
	#define BUTTON_SPEED_M 					0x00001
	#define BUTTON_SPEED_P 					0x00002
	#define BUTTON_FORWARD 					0x00004
	#define BUTTON_BACKWARD 				0x00008
	#define BUTTON_CONFIG_SCREEN 		0x00010
	#define BUTTON_ACCEL_M 					0x00020
	#define BUTTON_ACCEL_P 					0x00040
	#define BUTTON_CABLE_M 					0x00080
	#define BUTTON_CABLE_P 					0x00100
	#define BUTTON_CONFIG 					0x00200
	#define SCREEN_PRESSED 					0x00400
	#define BUTTON_CONTROL_SCREEN 	0x00800

	/******************************************************************************/
	/*            										Functions            	  				            */
	/******************************************************************************/ 
	
	/**
	 * @brief  
	 * @param  data: 
	 * @retval None
	 */	
	void CamScreen_Init(void);
	
	/**
	 * @brief  
	 * @param  data: 
	 * @retval None
	 */	
	void CamScreen_EcranAccueil(void);

	/**
	 * @brief  
	 * @param  data: 
	 * @retval None
	 */	
	void CamScreen_EcranConfig(void);

	/**
	 * @brief  
	 * @param  data: 
	 * @retval None
	 */	
	void CamScreen_EcranControle(void);

	/**
	 * @brief  
	 * @param  data: 
	 * @retval None
	 */	
	void CamScreen_RefreshEcranConfig(T_Config_Setting* Config_Setting);

	/**
	 * @brief  
	 * @param  data: 
	 * @retval None
	 */	
	void CamScreen_RefreshEcranControle(T_Controle_Information* Controle_Information);

	/**
	 * @brief  
	 * @param  data: 
	 * @retval None
	 */	
	void CamScreen_ClrScreen(void);
	
	/**
	 * @brief  
	 * @param  data: 
	 * @retval None
	 */	
	uint16_t CamScreen_ButtonsState(void);
	
#endif

