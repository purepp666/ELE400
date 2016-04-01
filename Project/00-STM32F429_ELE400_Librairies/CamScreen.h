#ifndef ECRAN_IMAGE_H
#define ECRAN_IMAGE_H


#include "defines.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_stmpe811.h"
#include "tm_stm32f4_ili9341_button.h"
#include "tm_stm32f4_disco.h"
//#include "button_back.h"
#include <stdio.h>

#include "CamProcessScreen.h"


extern void EcranAccueil(void);

extern void EcranConfig(void);

extern void EcranControle(void);

extern void RefreshEcranConfig(T_Config_Setting* Config_Setting);

extern void RefreshEcranControle(T_Controle_Information* Controle_Information);

extern void ClrScreen(void);
#endif

