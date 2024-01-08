	/*
 * lib_motor.h
 *
 *  Created on: Aug 30, 2023
 *      Author: BaoThuan
 */

#ifndef LIB_MOTOR_H_
#define LIB_MOTOR_H_
/*include lib*/
#include "stm32f1xx_hal.h"
#include<Lib_Sensor.h>

typedef enum
{
	RIGHT_UP = 10,
	RIGHT_DOWN,
	LEFT_UP,
	LEFT_DOWN,
	STOP
} DIR_MOTOR;

extern uint8_t quick_speed_right;
extern uint8_t quick_speed_left;
/*Timer init*/
	extern TIM_HandleTypeDef* handlerTimerInLib_right;
	extern TIM_HandleTypeDef* handlerTimerInLib_left;
/*Channel timer init*/
	extern uint32_t Channel_TIM_right;
	extern uint32_t Channel_TIM_left;
/*port dir init*/
	extern GPIO_TypeDef * Port_DIR_Right;
	extern GPIO_TypeDef * Port_DIR_Left;
/*pin dir init*/
	extern uint16_t Pin_DIR_Right;
	extern uint16_t Pin_DIR_Left;
/*Macro function for Motor*/
	void TimerInit(TIM_HandleTypeDef * timerHandler_right, TIM_HandleTypeDef * timerHandler_left, 	\
						uint32_t Channel_R, uint32_t Channel_L, 									\
						GPIO_TypeDef *GPIO_R, uint16_t GPIO_Pin_R, 									\
						GPIO_TypeDef *GPIO_L, uint16_t GPIO_Pin_L);									\
	/*DIR Motor*/
		void motor_move_4_Dir(DIR_MOTOR Dir_motor);
		void motor_right_move(int speed_r);
		void motor_right_down(int speed_r);
		void motor_left_move(int speed_l);
		void motor_left_down(int speed_l);
		void motor_stop();
	/*Control motor: DIR, PWM*/
		void motor_down(int speed, uint32_t time_down);
		void motor_move(int speedr, int speedl);
		void motor_control (int speed_right, int speed_left); //forward_brake(motor_speed_increase, motor_speed_decread)
		void forward_brake(int pos_right, int pos_left);

		void quick_turn();
#endif /* LIB_MOTOR_H_ */
