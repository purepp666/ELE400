/**
 * @author  
 * @email   
 * @version v1
 * @ide     Keil uVision
 * @brief   
 */

#include "CamProcessScreen.h"


#define ECRAN_ACCUEIL         0x0
#define ECRAN_CONFIG          0x1
#define ECRAN_CONTROLLER      0x2
#define ECRAN_ARRETE_URGENCE  0x4

extern uint8_t Status_Ecran_;

/******************************************************************************/
/*            									Global Variables           	      			 		  */
/******************************************************************************/ 

T_Controle_Information controle_info_={0,0,0,0,0,0};
T_Config_Setting setting_ = {0,0};

/******************************************************************************/
/*            								Functions definitions           	              */
/******************************************************************************/ 

void CamScreenP_Init(void){
	CamScreen_Init();
	CamScreen_EcranAccueil();
	Status_Ecran_ = ECRAN_ACCUEIL;
	while((CamScreen_ButtonsState() & SCREEN_PRESSED) == 0);
	CamScreen_EcranControle();
	Status_Ecran_ = ECRAN_ACCUEIL;
}




/******************************************************************************/

void CamScreenP_RefreshScreen(void){
}
