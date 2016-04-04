/**
 * @author  David Prairie-Bourgault and Alexi Demers
 * @email   alexidemers@gmail.com
 * @version v1
 * @ide     Keil uVision
 * @brief   This class controls the navigation between screen and the notifications
 *					to the main process when something happens that needs further processing.
 *					in summary, this library is the main process of the user interface's display.
 */

#include "CamProcessScreen.h"



#define INTRO_SCREEN         	0x0
#define CONFIG_SCREEN         0x1
#define CONTROL_SCREEN      	0x2
#define EMERG_STOP_SCREEN  		0x4

/******************************************************************************/
/*            										Defines	            	  				            */
/******************************************************************************/

/**
 * @brief 
 */
typedef struct {
	uint8_t accel;
	uint16_t cable_length;
} T_asked_settings;

/******************************************************************************/
/*            									Global Variables           	      			 		  */
/******************************************************************************/ 

// informations to update control screen
T_Controle_Information control_info_={Controller_Offline,controller_disconnected,0,0,0,0,0};
// informations to update config screen
T_Config_Setting setting_ = {0,0};
// information to set, asked by user
T_asked_settings asked_settings_ = {0,0};
// actual screen
uint8_t screen_status =INTRO_SCREEN;
// callback when something interesting happens on the screen
void (*event_callback_)(uint8_t screen) ;


/******************************************************************************/
/*            									Functions declarations     	      			 		  */
/******************************************************************************/ 

/**
 * @brief  Handles buttons and data changes in control screen
 * @param  None
 * @retval None
 */	
void CamScreenP_HandleControlScreen(void);

/**
 * @brief  Handles buttons and data changes in config screen
 * @param  None
 * @retval None
 */	
void CamScreenP_HandleConfigScreen(void);

/******************************************************************************/
/*            								Functions definitions           	              */
/******************************************************************************/ 

void CamScreenP_Init(void (*event_callback)(uint8_t screen)){
	CamScreen_Init();
	CamScreen_EcranAccueil();
	event_callback_ = event_callback;
}

/******************************************************************************/

void CamScreenP_RefreshScreen(void){
	
	switch(screen_status){
		case INTRO_SCREEN:
			if(CamScreen_ButtonsState() != 0){
				screen_status = CONTROL_SCREEN;
				CamScreen_EcranControle();
			}
			break;
		case CONFIG_SCREEN:
			CamScreenP_HandleConfigScreen();
			break;
		case CONTROL_SCREEN:
			CamScreenP_HandleControlScreen();
			break;
	}
}

/******************************************************************************/

void CamScreenP_HandleControlScreen(void){
	
	CamScreen_RefreshEcranControle(&control_info_);
	
	switch(CamScreen_ButtonsState()){
		case BUTTON_CONFIG_SCREEN:
			screen_status = CONFIG_SCREEN;
			CamScreen_EcranConfig();
			break;
		case BUTTON_FORWARD:
			if(control_info_.VitesseVoulu < 0){
				control_info_.VitesseVoulu = control_info_.VitesseVoulu*-1;
				event_callback_(CONTROL);
			}	
			break;
		case BUTTON_BACKWARD:
			if(control_info_.VitesseVoulu >= 0){
				control_info_.VitesseVoulu = control_info_.VitesseVoulu*-1;
				event_callback_(CONTROL);
			}
			break;
		case BUTTON_SPEED_P:
			if(control_info_.VitesseVoulu < 100){
				control_info_.VitesseVoulu++;
				event_callback_(CONTROL);
			}
			break;
		case BUTTON_SPEED_0:
			control_info_.VitesseVoulu = 0;
			event_callback_(CONTROL);
			break;	
		case BUTTON_SPEED_M:
			if(control_info_.VitesseVoulu > 0){
				control_info_.VitesseVoulu--;
				event_callback_(CONTROL);
			}
			break;	
		case BUTTON_CONNECT:
			event_callback_(CONNECT);
			break;		
	}
		
}

/******************************************************************************/

void CamScreenP_HandleConfigScreen(void){
	
	if(asked_settings_.accel == 0 && asked_settings_.cable_length == 0)
		CamScreen_RefreshEcranConfig(&setting_);
	else{
		setting_.Accelation = asked_settings_.accel;
		setting_.LenghtCable = asked_settings_.cable_length;
		CamScreen_RefreshEcranConfig(&setting_);
	}
	
	switch(CamScreen_ButtonsState()){
		case BUTTON_CONTROL_SCREEN:
			asked_settings_.accel = 0;
			asked_settings_.cable_length = 0;
			screen_status = CONTROL_SCREEN;
			CamScreen_EcranControle();
			break;
		case BUTTON_CONFIG:
			event_callback_(CONFIG);
			asked_settings_.accel = 0;
			asked_settings_.cable_length = 0;
			break;
		case BUTTON_CABLE_P:
			asked_settings_.cable_length++;
			break;
		case BUTTON_CABLE_M:
			if(asked_settings_.cable_length > 0)
				asked_settings_.cable_length--;
			break;
		case BUTTON_ACCEL_P:
			if(asked_settings_.accel < 5)
				asked_settings_.accel++;
			break;	
		case BUTTON_ACCEL_M:
			if(asked_settings_.accel > 1)
				asked_settings_.accel--;
			break;				
	}

}

/******************************************************************************/

void CamScreenP_ControllerOnline(bool state){
	if(state)
		control_info_.ControllerStatus = Controller_Online;
	else
		control_info_.ControllerStatus = Controller_Offline;
}

/******************************************************************************/

void CamScreenP_UpdateSpeed(int8_t speed){
	control_info_.VitesseReel = speed;
}

/******************************************************************************/

void CamScreenP_UpdateBattLevel(uint8_t level){
	control_info_.BattLevel = level;
}

/******************************************************************************/

void CamScreenP_UpdatePosition(uint16_t pos){
	control_info_.Position = pos;
}

/******************************************************************************/

void CamScreenP_UpdateAccel(uint8_t accel){
	setting_.Accelation = accel;
}

/******************************************************************************/

void CamScreenP_UpdateCableLenght(uint16_t lenght){
	setting_.LenghtCable = lenght;
}

/******************************************************************************/

void CamScreenP_Errors(uint16_t flags){
	control_info_.errors_flags = 0;
	control_info_.errors_flags |= flags;
}

/******************************************************************************/

void CamScreenP_SetConnected(bool state){
	if(state)
		control_info_.ControllerConnected = controller_connected;
	else
		control_info_.ControllerConnected = controller_disconnected;
}

/******************************************************************************/

uint8_t CamScreenP_GetAccel(void){
	return asked_settings_.accel;
}

/******************************************************************************/

uint16_t CamScreenP_GetCableLenght(void){
	return asked_settings_.cable_length;
}

/******************************************************************************/

int8_t CamScreenP_GetSpeed(void){
	return control_info_.VitesseVoulu;
}

/******************************************************************************/
