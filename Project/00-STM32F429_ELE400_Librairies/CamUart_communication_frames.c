/**
 * @author  Alexi Demers
 * @email   alexidemers@gmail.com
 * @version v1
 * @ide     Keil uVision
 * @brief   Communication frames library for STM32F429. This library handles
 *					sending messages and receiving messages from the embedded controller
 *					of the cable cam. The wireless communication module used is 
 *					Xbee.
 */
 
#include <stddef.h> 
#include "CamUart_communication_frames.h"
#include "tm_stm32f4_usart.h"

/******************************************************************************/
/*            											Defines           	      				        */
/******************************************************************************/ 

#define BAUDRATE 					115200
#define CONNECT_SOF 			"CXN"
#define DISCONNECT_SOF 		"DCX"
#define CONFIG_SOF 				"CNF"
#define CONTROL_SOF 			"CNT"
#define RX_MSG_SOF 				"STA"
#define RX_BUF_SIZE 			25
#define USART_USED				USART1 // The interrupt handler must correspond
#define PINS_USED					TM_USART_PinsPack_2

/**
 * Interrupt handler state machine
 */
enum state_machine{
	e_sof0 = 0,
	e_sof1,
	e_sof2,
	e_batt_lvl,
	e_posH,
	e_posL,
	e_ai_speed,
	e_ac_speed,
	e_accel,
	e_decel,
	e_clenghtH,
	e_clenghtL,
	e_interface_add,
	e_errors,
	e_checksum
};


/******************************************************************************/
/*            									Global Variables           	      			 		  */
/******************************************************************************/ 

uint8_t this_device_address_ = 0;
CamRxData rx_buffer_[RX_BUF_SIZE];
uint8_t rx_buffer_level_ = 0;
uint8_t error_count_ = 0;



/******************************************************************************/
/*            									Private functions           	      				  */
/******************************************************************************/ 

/**
 * @brief  Calculates the checksum for the input frame. This checksum must be
 *					inserted at the end of the frame
 * @param  frame: frame to calculate
 * @param  lenght: lenght of the data to calculate. 
 * @retval None
 */	
uint8_t CamUart_CalculateChecksum(uint8_t *frame, uint8_t lenght);



/******************************************************************************/
/*            								Functions definitions           	              */
/******************************************************************************/ 

void CamUart_Init(uint8_t this_device_address){
	
	TM_USART_Init(USART_USED, TM_USART_PinsPack_1, BAUDRATE);
	
	this_device_address_ = this_device_address;
}

/******************************************************************************/
 
void CamUart_SendConnectFrame(void){
	uint8_t frame[5] = {CONNECT_SOF[0], CONNECT_SOF[1], CONNECT_SOF[2]};
	
	frame[3] = this_device_address_;
	frame[4] = CamUart_CalculateChecksum(frame, 4);								
	
	TM_USART_Send(USART_USED, frame, 5);
}

/******************************************************************************/
 
void CamUart_SendDisconnectFrame(void){
	uint8_t frame[5] = {DISCONNECT_SOF[0], DISCONNECT_SOF[1], DISCONNECT_SOF[2]};
	
	frame[3] = this_device_address_;
	frame[4] = CamUart_CalculateChecksum(frame, 4);								
	
	TM_USART_Send(USART_USED, frame, 5);
}

/******************************************************************************/
 
void CamUart_SendConfigFrame(const ConfigCommand data){
	uint8_t frame[9] = {CONFIG_SOF[0], CONFIG_SOF[1], CONFIG_SOF[2]};
	
	frame[3] = this_device_address_;
	frame[4] = data.accel;
	frame[5] = data.decel;
	frame[6] = (uint8_t)(data.cable_lenght >> 8);
	frame[7] = (uint8_t)data.cable_lenght;
	frame[8] = CamUart_CalculateChecksum(frame, 8);								
	
	TM_USART_Send(USART_USED, frame, 9);
}

/******************************************************************************/
 
void CamUart_SendControlFrame(const ControlCommand data){
	uint8_t frame[7] = {CONTROL_SOF[0], CONTROL_SOF[1], CONTROL_SOF[2]};
	
	frame[3] = this_device_address_;
	frame[4] = data.speed;
	frame[5] = (uint8_t)data.emergency_stop;
	frame[6] = CamUart_CalculateChecksum(frame, 5);								
	
	TM_USART_Send(USART_USED, frame, 6);
}

/******************************************************************************/

uint8_t CamUart_CalculateChecksum(uint8_t *frame, uint8_t lenght){
	uint8_t i = 0;
	uint8_t checksum = 0;
	
	// loops until the end of the string
	for(i = 0; i < lenght; i++){
		checksum += frame[i];
	}
	return checksum;
}

/******************************************************************************/

uint8_t CamUart_GetRxErrorsCount(){
	uint8_t retval = error_count_;
	error_count_ = 0;
	return retval;
}

/******************************************************************************/

bool CamUart_ReadMessage(CamRxData *data){
	
	uint8_t i = 0;
	
	// if buffer isn't empty
	if(rx_buffer_level_){
		// saves the oldest data received as the data to return
		*data = rx_buffer_[0];

		// shift datas in the buffer to remove one
		for(i = 0; i < rx_buffer_level_ - 1; i++){
			rx_buffer_[i] = rx_buffer_[i+1];
		}
		// indicates the buffer has one data removed
		rx_buffer_level_--;
		return true;
	}	
	else
	{
		return false;
	}
}

/******************************************************************************/
// Interrupt handler
void TM_USART1_ReceiveHandler(uint8_t c){
	static uint8_t state = 0;
	uint8_t sum =0;
	
	switch(state){
		// rcvd 1st SOF char?
		case e_sof0:
			if(c == RX_MSG_SOF[0]){
				state++;
			}
			break;
		// rcvd 2nd SOF char?	
		case e_sof1:
			if(c == RX_MSG_SOF[1])
				state++;
			else
				state = 0;
			break;
		// rcvd 3rd SOF char?	
		case e_sof2:
			if(c == RX_MSG_SOF[2])
				state++;
			else
				state = 0;
			break;
			
		// saves all next datas	
		case e_batt_lvl:
			rx_buffer_[rx_buffer_level_].battery_level = c;
			state++;
			break;
		case e_posH:
			rx_buffer_[rx_buffer_level_].position = (uint16_t)(c << 8);
			state++;
			break;
		case e_posL:
			rx_buffer_[rx_buffer_level_].position |= c;
			state++;
			break;
		case e_ai_speed:
			rx_buffer_[rx_buffer_level_].aimed_speed = c;
			state++;
			break;
		case e_ac_speed:
			rx_buffer_[rx_buffer_level_].actual_speed = c;
			state++;
			break;
		case e_accel:
			rx_buffer_[rx_buffer_level_].accel = c;
			state++;
			break;
		case e_decel:
			rx_buffer_[rx_buffer_level_].decel = c;
			state++;
			break;
		case e_clenghtH:
			rx_buffer_[rx_buffer_level_].cable_lenght = c << 8;
			state++;
			break;
		case e_clenghtL:
			rx_buffer_[rx_buffer_level_].cable_lenght |= c;
			state++;
			break;
		case e_interface_add:
			rx_buffer_[rx_buffer_level_].connected_interface_address = c;
			state++;
			break;		
		case e_errors:
			rx_buffer_[rx_buffer_level_].error = c;
			state++;
			break;
		
		// calculating checksum
		case e_checksum:
		
			sum += RX_MSG_SOF[0]+RX_MSG_SOF[1]+RX_MSG_SOF[2];
			sum += rx_buffer_[rx_buffer_level_].battery_level;
			sum += rx_buffer_[rx_buffer_level_].position >> 8;
			sum += (uint8_t)rx_buffer_[rx_buffer_level_].position;
			sum += rx_buffer_[rx_buffer_level_].aimed_speed;
			sum += rx_buffer_[rx_buffer_level_].actual_speed;
			sum += rx_buffer_[rx_buffer_level_].accel;
			sum += rx_buffer_[rx_buffer_level_].decel;
			sum += rx_buffer_[rx_buffer_level_].cable_lenght >> 8;
			sum += (uint8_t)rx_buffer_[rx_buffer_level_].cable_lenght;
			sum += rx_buffer_[rx_buffer_level_].connected_interface_address;
			sum += rx_buffer_[rx_buffer_level_].error;

			// if checksum passes, increases buffer size. Else, increases error count
			if(sum == c){
				// if buffer overflows
				if(rx_buffer_level_ >= (RX_BUF_SIZE - 1)){
					error_count_++;
				}
				else{
					rx_buffer_level_++;
				}
			}		
			else{
				error_count_++;
			}
		
			state = 0;
			break;
	}
}
/******************************************************************************/
