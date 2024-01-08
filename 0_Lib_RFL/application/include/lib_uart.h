/*
 * lib_uart.h
 *
 *  Created on: Aug 30, 2023
 *      Author: BaoThuan
 */

#ifndef LIB_UART_H_
#define LIB_UART_H_

#include "stm32f1xx_hal.h"
#include<string.h>
#include<stdio.h>
#include<Lib_Controller.h>
#include<Lib_Sensor.h>
#include<lib_motor.h>

typedef enum
{
	START_ROBOT = 0,
	STOP_ROBOT,
	CHECK_THRESHOLD, //threshold
	START_MOTOR,
	POWER
} STATE_ROBOT;

//************
//UART *********
//*************
extern float kx[3];
extern STATE_ROBOT State_Robot;


void bluetooth_init_UART(UART_HandleTypeDef *huart);
void bluetooth_Return_parameter(uint8_t state_tx_lv, uint8_t speed_debug_right, uint8_t speed_debug_left, float * Kx, uint8_t * sensor_read);
/*UART_Bluetooth check string and act*/
STATE_ROBOT bluetooth_check_string_stop_start_robot(uint8_t * rxBuffer);
void bluetooth_rx_change_kx(uint8_t *rxBuffer);
uint8_t Char2Num(char * var_char);
void change_line_sensor_read(uint8_t *rxBuffer);

#endif /* LIB_UART_H_ */
