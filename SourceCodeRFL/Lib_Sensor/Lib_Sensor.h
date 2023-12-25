/*
 * Lib_Sensor.h
 *
 *  Created on: Oct 5, 2023
 *      Author: BaoThuan
 */

#ifndef LIB_SENSOR_H_
#define LIB_SENSOR_H_

#include "stm32f1xx_hal.h"
#include <lib_uart.h>
//#include <lib_motor.h>
/*Define PIN 74HC4051*/
typedef enum
{
    S0 = 0,
	S1 = 1,
	S2 = 2,
	S3 = 4,
	S4 = 6,
	S5 = 7,
	S6 = 5,
	S7 = 3
} IC74HC4051_S_PIN;
typedef enum
{
    X0 = 0,
	X1,
	X2,
	X3,
	X4,
	X5,
	X6,
	X7
} IC74HC595_S_PIN;
typedef enum
{
	LINE_ERROR = 8,
	LINE_VALID
} LINE_STATE;
typedef enum
{
	DONT_KNOW = 0,
	END_LEFT = 1 ,
	END_RIGHT = 2,
	END_MIDDLE = 4
} LINE_BREAKDOW;
typedef enum
{
	LINE_WHITE = 0,
	LINE_BLACK
} LINE_STATE_COLOR;
///*adc init*/
//	extern ADC_HandleTypeDef 			hadc_lib;
/*Private Val*/
	extern LINE_BREAKDOW last_end;
	extern uint16_t last_idle;
	extern uint8_t active;
	extern uint8_t flat_define_threshold;
	extern uint16_t threshold[7];
	extern LINE_STATE_COLOR  state_line_check;
/*74HC4051 VAL*/
	extern GPIO_TypeDef * 				IC74HC4051_GPIOx;
	extern uint16_t 					IC74HC4051_S0_Pin;
	extern uint16_t						IC74HC4051_S1_Pin;
	extern uint16_t 					IC74HC4051_S2_Pin;
/*74HC595 VAL*/
	/*74HC595 Port*/
	extern GPIO_TypeDef *				IC74HC595_ST_Port;
	extern GPIO_TypeDef * 				IC74hc595_DS_Port;
	extern GPIO_TypeDef * 				IC74HC595_Shift_Port;
	/*74HC595 Pin*/
	extern uint16_t 					IC74hc595_Shift_Pin;
	extern uint16_t						IC74hc595_DS_Pin;
	extern uint16_t						IC74HC595_ST_Pin;
/*Value user*/
	extern uint16_t pos_sensor;
	extern uint16_t old_pos_sensor;
	extern uint8_t err_line;
	extern uint16_t *	ADC_get_rawValue_sensor;
/*MACRO FUNCTION*/
	/*Macro function of 74hc4051*/
		void IC74HC4051_Init(ADC_HandleTypeDef handler, GPIO_TypeDef * GPIO, uint16_t GPIO_Pin_S0, uint16_t GPIO_Pin_S1, uint16_t GPIO_Pin_S2);
		GPIO_PinState IC74hc4051_BitRead(uint8_t channel_01, uint8_t index);
		void IC74hc4051_SelectChannel(uint8_t channel);
		uint16_t* IC74hc4051_Read_rawADC_1Chan(uint8_t i);
		uint16_t* IC74hc4051_Read_rawADC_7Chan();
	/*Macro function of 74hc595*/
		void IC74HC595_Init(GPIO_TypeDef * DS_Port, GPIO_TypeDef * Shift_Port, GPIO_TypeDef * ST_Port, uint16_t DS_Pin, uint16_t SH_Pin, uint16_t ST_Pin);
		void IC74HC595(uint16_t  tt);
	/*Macro function*/
		LINE_STATE Check_error(uint16_t F_pos_SS);
		uint16_t define_data(uint16_t* threshold);
	/*Sensor define threshold*/
		uint16_t * define_threshold(uint16_t* ADC_get_rawValue_sensor);
		void blink_led(uint8_t tt);
		uint16_t * action_define_threshold();
#endif /* LIB_SENSOR_H_ */
