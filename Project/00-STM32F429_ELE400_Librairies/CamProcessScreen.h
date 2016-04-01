#ifndef CAM_PROCESS_SCREEN_H
#define CAM_PROCESS_SCREEN_H

#include <stdbool.h>
#include "stm32f4xx.h"
#include "defines.h"

#define NombreErreur 8 // nombre de différentes erreurs possibles.

extern uint16_t Fleche_Haut_18x23[];


extern uint16_t Fleche_Bas_18x23[];



extern uint16_t Fleche_Droite_23x18[];


extern uint16_t Fleche_Gauche_23x18[];


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


void Init(T_Config_Setting *setting, T_Controle_Information * controle_info);


#endif
