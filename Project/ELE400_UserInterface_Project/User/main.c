/**
 *	Keil project for ELE400 cablecam user interface using STM32f429I discovery.
 *
 *
 *	@author		Alexi Demers
 *	@email		alexidemers@gmail.com
 *	@ide		Keil uVision 5
 *	@packs		STM32F4xx Keil packs version 2.2.0 or greater required
 *	@stdperiph	STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */

#include "stm32f4xx.h"

#include "defines.h"
#include "tm_stm32f4_delay.h"
#include "../00-STM32F429_ELE400_Librairies/CamUart_communication_frames.h"


#define DEBUG

#ifdef DEBUG
	#include "tm_stm32f4_usart.h"
#endif

/******************************************************************************/
/*            												Defines     	      			 		  				*/
/******************************************************************************/ 

#define DISCOVERY_DELAY 			5			// x 0.1 sec
#define ERROR_RECOVERY_COUNT 	200		// x 0.1 sec
#define ADDRESS 							0x35

// possible errors
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
/*            									Functions declarations     	      			 		  */
/******************************************************************************/ 

/**
 * @brief  Handles timings in the main thread
 * @param  param: unused
 * @retval None
 */	
void CustomTimerHandler(void* param);

/**
 * @brief  Process messages and sets screen values
 * @param  rx_data: data received for uart
 * @retval None
 */	
void ProcessMessages(CamRxData rx_data);

/**
 * @brief  Process errors conatined in messages
 * @param  rx_data: data received for uart
 * @retval true if an error was detected
 */	
bool HandleTxErrors(CamRxData rx_data);

/**
 * @brief  Process rx communication errors
 * @param  none
 * @retval true if an error was detected
 */	
bool HandleRxErrors(void);

/**
 * @brief  Deletes encountered errors
 * @param  none
 * @retval none
 */	
void RecoverFromErrors(void);

/**
 * @brief  Sends a uart command. 
 * @param  command: See CamScreen_display.h for button defines
 * @retval none
 */	
void SendCommand(uint8_t command);

/******************************************************************************/
/*            									Global Variables           	      			 		  */
/******************************************************************************/ 

uint16_t errors_flags = 0;
uint32_t timer_counter_ = DISCOVERY_DELAY;
uint32_t error_reset_counter_ = 0;
ConfigCommand conf_command;
ControlCommand cont_command;

/******************************************************************************/
/*            									      Main           	      			 		  			*/
/******************************************************************************/ 

int main(void) {
	
	CamRxData rx_data;									// Data read from uart
	bool errors_encountered = false;		// Indicates an error has been encountered
	TM_DELAY_Timer_t *timer;						// timer used
	
	SystemInit();
	
	// Init interface address
	CamUart_Init(ADDRESS);
	
	// Start timers
	TM_DELAY_Init();
	TM_DELAY_EnableDelayTimer();
	
	// Creates new timer
	timer = TM_DELAY_TimerCreate(100, 1, 1, &CustomTimerHandler,0);

	while(1){
		
		/**************** Read XBee messages ************************************/
		if(CamUart_ReadMessage(&rx_data)){
			
			// If controller just appeared online, sets initial values
			if(timer_counter_ >= DISCOVERY_DELAY){
				conf_command.accel = rx_data.accel;
				conf_command.decel = rx_data.decel;
				conf_command.cable_lenght = rx_data.cable_lenght;
				cont_command.speed = rx_data.aimed_speed;
			}
				
			timer_counter_ = 0;	// Indicates the cable cam is communicating
			ProcessMessages(rx_data);
			
			// Tx errors are detected when received values does not correspond
			// to sent values
			errors_encountered = HandleTxErrors(rx_data);
			
			#ifdef DEBUG
				TM_USART_Puts(USART1,"\nMessage correctly received");
				TM_USART_Puts(USART1,"\nBatt level: ");
				TM_USART_Send(USART1,&rx_data.battery_level,1);
				TM_USART_Puts(USART1,"\nSpeed: ");
				TM_USART_Send(USART1,(uint8_t*)&rx_data.actual_speed,1);
				TM_USART_Puts(USART1,"\nAsked speed: ");
				TM_USART_Send(USART1,(uint8_t*)&rx_data.aimed_speed,1);
				TM_USART_Puts(USART1,"\nAccel: ");
				TM_USART_Send(USART1,&rx_data.accel,2);
				TM_USART_Puts(USART1,"\nDecel: ");
				TM_USART_Send(USART1,&rx_data.decel,1);
				TM_USART_Puts(USART1,"\nPosition: ");
				TM_USART_Send(USART1,(uint8_t*)&rx_data.position,2);
				TM_USART_Puts(USART1,"\nCable Length: ");
				TM_USART_Send(USART1,(uint8_t*)&rx_data.cable_lenght,2);
			#endif
		}
		
		
		errors_encountered = HandleRxErrors();
		
		
		
		/**************** Handles errors recovery *******************************/
		
		
		if(errors_encountered){
			error_reset_counter_ = 0;
			#ifdef DEBUG
				TM_USART_Puts(USART1,"Errors found\n");
			#endif
		}
			
		// If no errors have been received in a while
		if(error_reset_counter_ >= ERROR_RECOVERY_COUNT){
			error_reset_counter_ = 0;
			// Stops showing errors 
			RecoverFromErrors();
			
			#ifdef DEBUG
				TM_USART_Puts(USART1,"No errors received in a while. reseting errors\n");
			#endif
		}
		
		
		/**************** Handles communication verification*********************/
		
		// If controller hasn't communicated in a while
		if(timer_counter_ >= DISCOVERY_DELAY){
			// Stops timer
			TM_DELAY_TimerStop(timer);
			// Show controller is offline
			//CamLcd_ControllerOnline(false)

		}
		else{
			// Starts timer
			TM_DELAY_TimerStart(timer);
			//CamLcd_ControllerOnline(true)
			

		}
		
		/******************** Touch screen and tx ********************************/
		
		// if(CamLcd_ReadTouchScreenButtons(&button_pressed)){
		//		SendCommand(button_pressed);	
		//		#ifdef DEBUG
		//			TM_USART_Puts(\n\nUSART1,"Touch Screen button: ");
		//			TM_USART_Send(USART1,&button_pressed,1);
		//		#endif
	  // }
			
		
		
		/********************** Screen Thread ************************************/

		//CamLcd_RefreshScreen();

	}
	
}



/******************************************************************************/
/*            							Functions definitions           	     		  			*/
/******************************************************************************/ 

void CustomTimerHandler(void* param){
	timer_counter_++;
	error_reset_counter_++;
}

/******************************************************************************/

void ProcessMessages(CamRxData rx_data){
	
	//CamLcd_UpdateSpeed(rx_data.actual_speed);
	//CamLcd_UpdateBattLevel(rx_data.battery_level)
	//CamLcd_UpdatePosition(rx_data.position)
	
	// Verify if we are connected to the controller
	if(ADDRESS == rx_data.connected_interface_address){
		// CamLcd_SetConnected(true);
	}
	else{
		// CamLcd_SetConnected(false);
	}

}

/******************************************************************************/

bool HandleTxErrors(CamRxData rx_data){
	bool error_encountered = false;
	static bool tx_error = false;
	
	
	// If an error that hasn't been seen yet occurs on cablecam
	if(rx_data.error != 0 && (rx_data.error & errors_flags)!=rx_data.error){
		
		error_reset_counter_=0; // Resets the error recovery timer
	
		// Deletes errors that have already been seen
		rx_data.error = rx_data.error & !errors_flags;
		
		// Raises new flags
		errors_flags |= rx_data.error;
	
		if(rx_data.error & CABLE_END){
			//CamLcd_AddError("End of cable reached");
		}
		if(rx_data.error & BATT_TEMP){
			//CamLcd_AddError("Battery temperature high");
		}
		if(rx_data.error & BATT_LOW){
			//CamLcd_AddError("Battery low");
		}
		if(rx_data.error & MOTOR_FORCES){
			//CamLcd_AddError("Motor forces to reach speed");
		}
		if(rx_data.error & INVALID_COMMAND){
			tx_error = true;
		}
		if(rx_data.error & OBSTACLE){
			//CamLcd_AddError("Obstacle on the cable");
		}
		if(rx_data.error & EMERGENCY_STOP){
			//CamLcd_AddError("Emergency stop enabled");
		}
		if(rx_data.error & INTERFACES_CONFLICT){
			//CamLcd_AddError("Other interfaces try to connect");
		}
	}
	
	
	// If a config is different from expected
	if((conf_command.accel != rx_data.accel)
		|| (conf_command.decel != rx_data.decel)
		|| (conf_command.cable_lenght != rx_data.cable_lenght) 
		|| (cont_command.speed != rx_data.aimed_speed)){
		// Sets an error flag
		error_encountered = true; 
		tx_error = true;
	}
	
	// Verify and show tx errors
	if(tx_error && (errors_flags & TX_ERROR) == 0){
		errors_flags |= TX_ERROR;
		// CamLcd_AddError("A command was lost by the cablecam. Verify config.");
	}

	return error_encountered;
}

/******************************************************************************/

bool HandleRxErrors(void){
	bool error_encountered = false;
	uint8_t errors = CamUart_GetRxErrorsCount();
	
	// Verify and show rx errors
	if(errors && (errors_flags & RX_ERROR) == 0){
		errors_flags |= RX_ERROR;
		// CamLcd_AddError("Invalid messages received");
	}
	
	if(errors)
		error_encountered = true;

	return error_encountered;
}

/******************************************************************************/

void RecoverFromErrors(void){
	// CamLcd_DeleteErrors(); 
	errors_flags = 0;
}

/******************************************************************************/

void SendCommand(uint8_t command){
	/*switch(command){
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
	}*/
}

/******************************************************************************/
