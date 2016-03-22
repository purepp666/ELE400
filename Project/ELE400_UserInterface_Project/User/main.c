/**
 *	Keil project for USART using strings
 *
 *  Before you start, select your target, on the right of the "Load" button
 *
 *	@author		Tilen Majerle
 *	@email		tilen@majerle.eu
 *	@website	http://stm32f4-discovery.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */

#include "stm32f4xx.h"

#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"
#include "../00-STM32F429_ELE400_Librairies/ele400_communication_frames.h"

/******************************************************************************/
/*            												Defines     	      			 		  				*/
/******************************************************************************/ 

#define DISCOVERY_DELAY 5
#define ERROR_RECOVERY_COUNT 100
#define ADDRESS 0x35

// possible errors
#define NO_ERROR 						0
#define CABLE_END 					1
#define BATT_TEMP 					2
#define BATT_LOW 						4
#define MOTOR_FORCES 				8
#define INVALID_COMMAND 		16
#define OBSTACLE 						32
#define EMERGENCY_STOP 			64
#define INTERFACES_CONFLICT 128



/******************************************************************************/
/*            									Functions declarations     	      			 		  */
/******************************************************************************/ 

void CustomTimerHandler(void* param);

/******************************************************************************/
/*            									Global Variables           	      			 		  */
/******************************************************************************/ 

uint32_t timer_counter_ = 0;
uint32_t counter_compare_ = 0;
uint32_t error_reset_counter_ = 0;

/******************************************************************************/
/*            									      Main           	      			 		  			*/
/******************************************************************************/ 

int main(void) {
	
	bool tx_error = false;
	CamRxData rx_data;
	ConfigCommand conf_command;
	ControlCommand cont_command;
	TM_DELAY_Timer_t *timer;
	
	SystemInit();
	
	// Init interface address
	CamUart_Init(ADDRESS);
	
	// Start timers
	TM_DELAY_Init();
	TM_DELAY_EnableDelayTimer();
	
	// Creates new timer
	timer = TM_DELAY_TimerCreate(100, 1, 1, &CustomTimerHandler,0);
	
	// While no message has been received from the cable cam and 
	while((timer_counter_ < DISCOVERY_DELAY) || CamUart_ReadMessage(&rx_data));
	
	// Verify if the controller communicated 
	if(timer_counter_ < DISCOVERY_DELAY){
		// CamLcd_ControllerOnline(bool state)
	}
	else
	{
		// CamLcd_ControllerOnline(bool state)
	}
	
	timer_counter_ = 0;
	
	

	while(1){
		
		/**************** Read XBee messages ************************/
		if(CamUart_ReadMessage(&rx_data)){
			tx_error = false;
			
			// Updates screen
			/*
			CamLcd_UpdateSpeed(rx_data.actual_speed);
			CamLcd_UpdateBattLevel(rx_data.battery_level)
			CamLcd_UpdatePosition(rx_data.position)
			
			if(rx_data.error != 0){
				if(rx_data.error & CABLE_END){
					CamLcd_AddError("End of cable reached");
				}
				if(rx_data.error & BATT_TEMP){
					CamLcd_AddError("Battery temperature high");
				}
				if(rx_data.error & BATT_LOW){
					CamLcd_AddError("Battery low");
				}
				if(rx_data.error & MOTOR_FORCES){
					CamLcd_AddError("Motor forces to reach speed");
				}
				if(rx_data.error & INVALID_COMMAND){
					tx_error = true;
				}
				if(rx_data.error & OBSTACLE){
					CamLcd_AddError("Obstacle on the cable");
				}
				if(rx_data.error & EMERGENCY_STOP){
					CamLcd_AddError("Emergency stop enabled");
				}
				if(rx_data.error & INTERFACES_CONFLICT){
					CamLcd_AddError("Other interfaces try to connect");
				}
			}
			
			*/
			
			// If a config is different from expected
			if(conf_command.accel != rx_data.accel){
				// Sets an error flag
				tx_error = true;
			}
			if(conf_command.decel != rx_data.decel){
				tx_error = true;
			}
			if(conf_command.cable_lenght != rx_data.cable_lenght){
				tx_error = true;
			}
			if(cont_command.speed != rx_data.aimed_speed){
				tx_error = true;
			}
			
			// Verify if we are connected to the controller
			if(ADDRESS == rx_data.connected_interface_address){
				// CamLcd_SetConnected(true);
			}
			else{
				// CamLcd_SetDisconnected(false);
			}
			
			// Verify and show errors
			if(tx_error){
				error_reset_counter_=0;
				// CamLcd_AddError("Data lost. Unable to config");
			}

			
			// HANDLE CONTROLLER ERRORS
			
			timer_counter_ = 0;
		}
		
		
		/**************** Handles communication verification*************/
		// If controller hasn't communicated in a while
		if(timer_counter_ >= DISCOVERY_DELAY){
			// Stops timer
			TM_DELAY_TimerStop(timer);
			// Show controller offline
			//CamLcd_ControllerOnline(false)
		}
		else{
			// Starts timer
			TM_DELAY_TimerStart(timer);
			//CamLcd_ControllerOnline(true)
		}
		
		
		/**************** Handles Rx errors *****************************/
		if(CamUart_GetRxErrorsCount()){
			error_reset_counter_=0;
			// CamLcd_AddError("Reception errors occured");
		}

		// If no errors have been received in a while
		if(error_reset_counter_ >= ERROR_RECOVERY_COUNT){
			// Stops errors showing
			error_reset_counter_ = ERROR_RECOVERY_COUNT;
			// CamLcd_DeleteErrors();
		}
			
		
		/******************** Touch screen *****************************/
		
		/* CamLcd_ReadTouchScreenButtons(button_pressed)---------
		
		switch(button_pressed){
			case CONNECT:
				CamUart_SendConnectFrame();
				break;
			case DISCONNECT:
				CamUart_SendDisconnectFrame();
				break;
			case CONFIGURE:
				conf_command.accel = CamLcd_GetAccel();-------
				conf_command.decel = CamLcd_GetDecel();-------
				conf_command.cable_lenght = CamLcd_GetCableLenght();----------
				CamUart_SendConfigFrame(conf_command);
				break;
			case LEFT_DIRECTION:
				cont_command.speed = -CamLcd_GetSpeed();
				cont_command.emergency_stop = false;
				CamUart_SendControlFrame(cont_command);
				break;
			case RIGHT_DIRECTION:
				cont_command.speed = CamLcd_GetSpeed();
				cont_command.emergency_stop = false;
				CamUart_SendControlFrame(cont_command);
				break;
		}
		*/
		
		// READ EMERGENCY BUTTON
		
		
		
	}
	
}

/******************************************************************************/
/*            							Functions definitions           	     		  			*/
/******************************************************************************/ 

void CustomTimerHandler(void* param){
	timer_counter_++;
	error_reset_counter_++;
}

