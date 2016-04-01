

#ifndef ECRAN_IMAGE_H
#define ECRAN_IMAGE_H
#include "defines.h"
#include "tm_stm32f4_ili9341.h"
#include "tm_stm32f4_stmpe811.h"
#include "tm_stm32f4_ili9341_button.h"
#include "tm_stm32f4_disco.h"
//#include "button_back.h"
#include <stdio.h>

#define NombreErreur 8 // nombre de différentes erreurs possibles.



typedef struct {
	uint8_t Accelation;      
	uint16_t LenghtCable;      
} T_Config_Setting;

typedef enum {
	Direction_Avant,
	Direction_Arriere
} T_Direction;

typedef enum {
	Controller_Offline,
	Controller_Online
} T_ControllerStatus;


typedef struct {
	char* StrErreur; //contient la string de l'erreur
	uint8_t ErreurPresente; //Mettre à 1 à l'emplacement correspondant si l'erreur est active sinon à 0;
} T_Erreur;


typedef struct {
	T_ControllerStatus ControllerStatus; //0->Offline 1->Online
	uint8_t BattLevel;
	T_Direction Direction; // 0->Avant 1->Arrière
	uint8_t VitesseVoulu;
	uint8_t VitesseReel;
	uint16_t Position;
	T_Erreur *TableauErreur[NombreErreur]; 
} T_Controle_Information;




extern void EcranAccueil(void);

extern void EcranConfig(void);

extern void EcranControle(void);

extern void RefreshEcranConfig(T_Config_Setting* Config_Setting);

extern void RefreshEcranControle(T_Controle_Information* Controle_Information);

extern void ClrScreen(void);
#endif

