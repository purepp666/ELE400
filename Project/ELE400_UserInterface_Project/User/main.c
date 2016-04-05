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
#include "tm_stm32f4_disco.h"
#include "../00-STM32F429_ELE400_Librairies/CamProcessScreen.h"


//#define DEBUG

#ifdef DEBUG
	#include "tm_stm32f4_usart.h"
#endif

/******************************************************************************/
/*            												Defines     	      			 		  				*/
/******************************************************************************/ 

#define DISCOVERY_DELAY 			20			// x 0.1 sec
#define ERROR_RECOVERY_COUNT 	200		// x 0.1 sec
#define ADDRESS 							0x7A
#define CLEAR_ERRORS					0x300




/******************************************************************************/
/*            									Functions declarations     	      			 		  */
/******************************************************************************/ 
/**
 * @brief  Event recevied from screen process
 * @param  screen : screen event
 * @retval None
 */	
void ScreenEvents(uint8_t screen);

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


/******************************************************************************/
/*            									Global Variables           	      			 		  */
/******************************************************************************/ 

uint16_t errors_flags_ = 0;
uint32_t timer_counter_ = DISCOVERY_DELAY;
uint32_t error_reset_counter_ = 0;
ConfigCommand conf_command;
ControlCommand cont_command;
bool connected_ = false;

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
	TM_DISCO_ButtonInit();
	
	// init screen and show intro screen
	CamScreenP_Init(&ScreenEvents);

	while(1){
	
		errors_encountered = 0;
		/**************** Read XBee messages ************************************/
		if(CamUart_ReadMessage(&rx_data)){
			
			// If controller just appeared online, sets initial values
			if(timer_counter_ >= DISCOVERY_DELAY){
				conf_command.accel = rx_data.accel;
				conf_command.cable_lenght = rx_data.cable_lenght;
				cont_command.speed = rx_data.aimed_speed;
			}
				
			timer_counter_ = 0;	// Indicates the cable cam is communicating
			ProcessMessages(rx_data);
			
			// Tx errors are detected when received values does not correspond
			// to sent values
			errors_encountered |= HandleTxErrors(rx_data);
			
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
				TM_USART_Puts(USART1,"\nPosition: ");
				TM_USART_Send(USART1,(uint8_t*)&rx_data.position,2);
				TM_USART_Puts(USART1,"\nCable Length: ");
				TM_USART_Send(USART1,(uint8_t*)&rx_data.cable_lenght,2);
			#endif
		}
		
		
		errors_encountered |= HandleRxErrors();
		
		
		
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
			// Stops the counter
			timer_counter_ = DISCOVERY_DELAY; 
			// Show controller is offline
			CamScreenP_ControllerOnline(false);

		}
		else{
			// Starts timer
			CamScreenP_ControllerOnline(true);
		}
		
		/******************** Emergency button **********************************/

		// Emergency button
		if(TM_DISCO_ButtonOnPressed()){
			cont_command.speed = CamScreenP_GetSpeed();
			cont_command.emergency_stop = true;
			CamUart_SendControlFrame(cont_command);
		}

		/********************** Screen Thread ************************************/

		CamScreenP_RefreshScreen();
	}
	
}



/******************************************************************************/
/*            							Functions definitions           	     		  			*/
/******************************************************************************/ 

void ScreenEvents(uint8_t screen){
	
	
	// If event is config event
	if(screen == CONFIG){
		conf_command.accel = CamScreenP_GetAccel();
		conf_command.cable_lenght = CamScreenP_GetCableLenght();
		CamUart_SendConfigFrame(conf_command);
	}
	// If event is control event
	if(screen == CONTROL){
		cont_command.speed = CamScreenP_GetSpeed();
		cont_command.emergency_stop = false;
		CamUart_SendControlFrame(cont_command);
	}
	// If event is connect event
	if(screen == CONNECT){
		if(connected_)
			CamUart_SendDisconnectFrame();
		else
			CamUart_SendConnectFrame();
	}

}

/******************************************************************************/

void CustomTimerHandler(void* param){
	timer_counter_++;
	error_reset_counter_++;
}

/******************************************************************************/

void ProcessMessages(CamRxData rx_data){
	
	CamScreenP_UpdateSpeed(rx_data.actual_speed);
	CamScreenP_UpdateBattLevel(rx_data.battery_level);
	CamScreenP_UpdatePosition(rx_data.position);
	CamScreenP_UpdateAccel(rx_data.accel);
	CamScreenP_UpdateCableLenght(rx_data.cable_lenght);
	
	// Verify if we are connected to the controller
	if(ADDRESS == rx_data.connected_interface_address){
		CamScreenP_SetConnected(true);
		connected_ = true;
	}
	else{
		CamScreenP_SetConnected(false);
		connected_ = false;
	}

}

/******************************************************************************/

bool HandleTxErrors(CamRxData rx_data){
		
	// Clears errors that do not need to be shown longer. Only communication
	// errors must stay on screen even if they are received only once.
	errors_flags_ &= CLEAR_ERRORS;
	// Raises new flags
	errors_flags_ |= rx_data.error;
	
	
	
	// If a config is different from expected
	if((conf_command.accel != rx_data.accel)
		|| (conf_command.cable_lenght != rx_data.cable_lenght) 
		|| (cont_command.speed != rx_data.aimed_speed)){
		// Sets an error flag
		errors_flags_ |= TX_ERROR;
	}

	CamScreenP_Errors(errors_flags_);
	
	if(errors_flags_)
		return true;
	else
		return false;

}

/******************************************************************************/

bool HandleRxErrors(void){
	uint8_t errors = CamUart_GetRxErrorsCount();
	
	// Verify and show rx errors
	if(errors){
		errors_flags_ |= RX_ERROR;
		CamScreenP_Errors(errors_flags_);
		return true;
	}
	else{
		CamScreenP_Errors(errors_flags_);
		return false;
	}
}

/******************************************************************************/

void RecoverFromErrors(void){
	errors_flags_ = 0;
}


/******************************************************************************/
