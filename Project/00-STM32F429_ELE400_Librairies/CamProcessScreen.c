/**
 * @author  
 * @email   
 * @version v1
 * @ide     Keil uVision
 * @brief   
 */

#include "CamProcessScreen.h"

/******************************************************************************/
/*            									Global Variables           	      			 		  */
/******************************************************************************/ 

T_Controle_Information controle_info_={0,0,0,0,0,0};
T_Config_Setting setting_ = {0,0};

/******************************************************************************/
/*            								Functions definitions           	              */
/******************************************************************************/ 

void CamScreenP_Init(){ 
	CamScreen_Init();
	CamScreen_EcranAccueil();
	while((CamScreen_ButtonsState() & SCREEN_PRESSED) == 0);
	CamScreen_EcranControle();
}

/******************************************************************************/

void CamScreenP_RefreshScreen(void){
	
}