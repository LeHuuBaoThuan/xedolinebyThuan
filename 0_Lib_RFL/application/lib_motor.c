/*
 * lib_motor.c
 *
 *  Created on: Aug 30, 2023
 *      Author: BaoThuan
 */
#include "lib_motor.h"
/*Variable dk*/
	uint8_t quick_speed_right = 30;
	uint8_t quick_speed_left = 50;

	uint8_t state_motor = 0;
/*Timer init*/
	TIM_HandleTypeDef* handlerTimerInLib_right;
	TIM_HandleTypeDef* handlerTimerInLib_left;
/*Channel timer init*/
	uint32_t Channel_TIM_right;
	uint32_t Channel_TIM_left;
/*port dir init*/
	GPIO_TypeDef * Port_DIR_Right;
	GPIO_TypeDef * Port_DIR_Left;
/*pin dir init*/
	uint16_t Pin_DIR_Right;
	uint16_t Pin_DIR_Left;
/* Phần code động cơ - thuật toán PID  */
	void TimerInit(TIM_HandleTypeDef * timerHandler_right, TIM_HandleTypeDef * timerHandler_left, 	\
						uint32_t Channel_R, uint32_t Channel_L, 									\
						GPIO_TypeDef *GPIO_R, uint16_t GPIO_Pin_R, 									\
						GPIO_TypeDef *GPIO_L, uint16_t GPIO_Pin_L)									\
	{
		handlerTimerInLib_left 	= 	timerHandler_left;
		handlerTimerInLib_right	= 	timerHandler_right;

		Channel_TIM_right		=	Channel_R;
		Channel_TIM_left		=	Channel_L;
		/*Port on lib_motor.c*/
			Port_DIR_Right			=	GPIO_R;
			Port_DIR_Left			=	GPIO_L;
		/*Pin on lib_motor.c*/
			Pin_DIR_Right			=	GPIO_Pin_R;
			Pin_DIR_Left			=	GPIO_Pin_L;
	}
/*Output speed motor*/
	void motor_control (int speed_right, int speed_left) 	//forward_brake(motor_speed_increase, motor_speed_decread)
	{
		int speed_motor_right_PV = speed_right;			 	// 0 <= |speed left or right| <= 100
		int speed_motor_left_PV = speed_left;

		if(speed_left < 0)
		{
			motor_left_down(100 - (- speed_motor_left_PV));
		}
		else
		{
			motor_left_move(100 - speed_motor_left_PV);
		}
		if(speed_right < 0)
		{
			motor_right_down(100 - (- speed_motor_right_PV));
		}
		else
		{
			motor_right_move(100 - speed_motor_right_PV);
		}
	}
	void quick_turn()
	{
//		static uint8_t last_end_pre = 0;

		if (last_idle <= 14) /*36ms last_idle + 1*/
		{
			if(state_motor == 1)
			{
				switch (last_end)
				{
					case END_MIDDLE:	/*SS giua la SS cuoi cung nhan vach den*/
						motor_control(40,32);
						break;
					case END_LEFT:		/*SS ben trai la SS cuoi cung nhan vach den*/
						motor_control(quick_speed_right - 5, -quick_speed_left);
						break;
					case END_RIGHT:		/*SS ben phai la SS cuoi cung nhan vach den*/
						motor_control(-quick_speed_left, quick_speed_right);
						break;
					default:
						__NOP();
						break;
				}
			}
			else
			{
				motor_stop(0);
			}
		}
		else if(last_idle <= 35) /*1.5s*/
		{

			if(state_motor == 1)
			{
				switch (last_end)
				{
					case END_MIDDLE:	/*SS giua la SS cuoi cung nhan vach den*/
						motor_control(40,32);
						break;
					case END_LEFT:		/*SS ben trai la SS cuoi cung nhan vach den*/
						motor_control(40, -20);
						break;
					case END_RIGHT:		/*SS ben phai la SS cuoi cung nhan vach den*/
						motor_control(-15, 45);
						break;
					default:
						__NOP();
						break;
//						speed_right_debug = 20;
//						speed_left_debug = -20;
				}
			}
			else
			{
				motor_stop(0);
			}
		}
		else
		{
			if(state_motor == 1)
			{
				switch (last_end)
				{
					case END_MIDDLE:	/*SS giua la SS cuoi cung nhan vach den*/
						motor_control(30,35);
						break;
					case END_LEFT:		/*SS ben trai la SS cuoi cung nhan vach den*/
						motor_control(-15, 45);
						break;
					case END_RIGHT:		/*SS ben phai la SS cuoi cung nhan vach den*/
						motor_control(40, -20);
						break;
					default:
						__NOP();
						break;
//						speed_right_debug = 30;
//						speed_left_debug = -30;
				}
			}
		}
	}
	void forward_brake(int pos_right, int pos_left)
	{
		if (active == 0)
		{
			quick_turn();
		}
		else if(active > 0)
		{
//			speed_right_debug = pos_right;
//			speed_left_debug = pos_left;
			if(state_motor == 1)
			{
				motor_control(pos_right, pos_left);
			}
			else
			{
				motor_stop(0);
			}
		}
	}
/*dir Motor right*/
	void motor_right_move(int speed_r)
	{
		HAL_GPIO_WritePin(Port_DIR_Right, Pin_DIR_Right, GPIO_PIN_RESET );
		__HAL_TIM_SET_COMPARE (handlerTimerInLib_right, Channel_TIM_right, speed_r);
	}
	void motor_right_down(int speed_r)
	{
		HAL_GPIO_WritePin(Port_DIR_Right, Pin_DIR_Right, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE (handlerTimerInLib_right, Channel_TIM_right, speed_r);
	}
/*dir Motor left*/
	void motor_left_move(int speed_l)
	{
		HAL_GPIO_WritePin(Port_DIR_Left, Pin_DIR_Left, GPIO_PIN_RESET);
		__HAL_TIM_SET_COMPARE (handlerTimerInLib_left, Channel_TIM_left, speed_l);
	}
	void motor_left_down(int speed_l)
	{
		HAL_GPIO_WritePin(Port_DIR_Left, Pin_DIR_Left, GPIO_PIN_SET);
		__HAL_TIM_SET_COMPARE (handlerTimerInLib_left, Channel_TIM_left, speed_l);
	}
/*dir Motor stop*/
	void motor_stop(uint32_t delay)
	{
		__HAL_TIM_SET_COMPARE (handlerTimerInLib_right, Channel_TIM_right, SPEED_STOP);
		__HAL_TIM_SET_COMPARE (handlerTimerInLib_left, Channel_TIM_left, SPEED_STOP);
		HAL_Delay(delay);
	}
/*dir Motor Move*/
	void motor_move(int speedr, int speedl)
	{
		motor_right_move(speedr);
		motor_left_move(speedl);
	}
	void motor_down(int speed, uint32_t time_down)
	{
		motor_right_down(speed);
		motor_left_down(speed);
		HAL_Delay(time_down);
	}
	/**/
	//LEHUUBAOTHUAN
