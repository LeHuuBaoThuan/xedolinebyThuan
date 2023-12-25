/*
 * Lib_Controller.h
 *
 *  Created on: Oct 23, 2023
 *      Author: BaoThuan
 */

#ifndef LIB_CONTROLLER_H_
#define LIB_CONTROLLER_H_

#include "stm32f1xx_hal.h"
#include<lib_motor.h>
#include<Lib_Sensor.h>
#include<lib_uart.h>

#define SPEED_STOP 0
#define T_1ms 0.001

extern int speed_right_debug;
extern int speed_left_debug;
extern uint8_t maxspeedl;
extern uint8_t maxspeedr;
extern uint8_t basespeedr;
extern uint8_t basespeedl;

extern uint8_t state_motor;

/*Macro func*/
	void past_errors (int error);
	int errors_sum (int index, int abs);
	void PID_control(float * Kx, uint16_t* threshold_func_PID);
	void Ne_vat_can___Cua_phai(uint8_t speed_Right, uint8_t speed_Left, uint32_t Time_Move, uint32_t Time_Right, uint32_t Time_Left);
#endif /* LIB_CONTROLLER_H_ */
