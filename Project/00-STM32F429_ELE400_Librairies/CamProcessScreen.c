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



/******************************************************************************/
/*            									Global Variables           	      			 		  */
/******************************************************************************/ 

T_Controle_Information controle_info_={1,100,40,30,400,0x3ff};
T_Config_Setting setting_ = {0,0};
uint8_t Status_Ecran_ =0;

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
	CamScreen_RefreshEcranControle(&controle_info_);
}




/******************************************************************************/

void CamScreenP_RefreshScreen(void){
}
