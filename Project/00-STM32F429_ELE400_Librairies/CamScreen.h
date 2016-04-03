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

#define NOMBRE_ERREUR 11 // nombre de différentes erreurs possibles.


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
	typedef enum {
		controller_connected,
		controller_disconnected
	} T_ControllerConnected;


	/**
	 * @brief 
	 */
	typedef struct {
		T_ControllerStatus ControllerStatus; //0->Offline 1->Online
		T_ControllerConnected ControllerConnected;
		uint8_t BattLevel;
		int8_t VitesseVoulu;
		int8_t VitesseReel;
		uint16_t Position;
		uint8_t errors_flags;
	} T_Controle_Information;
	
	/**
	 * @brief buttons pressed flags
	 */
	#define SCREEN_PRESSED					200
	
	#define BUTTON_CONFIG_SCREEN 		1
	#define BUTTON_FORWARD 					2
	#define BUTTON_BACKWARD 				3
	#define BUTTON_SPEED_P 					4
	#define BUTTON_SPEED_0 					5
	#define BUTTON_SPEED_M 					6
	#define BUTTON_CONNECT 					7
	
	#define BUTTON_CONTROL_SCREEN 	1
	#define BUTTON_CONFIG 					2
	#define BUTTON_CABLE_P 					3
	#define BUTTON_CABLE_M 					4
	#define BUTTON_ACCEL_P 					5
	#define BUTTON_ACCEL_M 					6
	
	
	/**
	 * @brief errors
	 */
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
	void CamScreen_ArretUrgence(void);

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
	uint8_t CamScreen_ButtonsState(void);
	
#endif

