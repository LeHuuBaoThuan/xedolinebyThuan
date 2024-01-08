/*
 * HC_SR04.h
 *
 *  Created on: Mar 23, 2022
 *      Author: Mr Ruler
 */

#ifndef INC_HC_SR04_H_
#define INC_HC_SR04_H_

#include "stm32f1xx_hal.h"

//#define TRIG_PIN GPIO_PIN_8
//#define TRIG_PORT GPIOB
//#define ECHO_PIN GPIO_PIN_9
//#define ECHO_PORT GPIOA

extern uint8_t Distance;

void HC_SR04_init(TIM_HandleTypeDef htim, uint32_t Channel, 			\
					HAL_TIM_ActiveChannel timer_active, uint32_t timer_it_cc, 			\
					GPIO_TypeDef * trig_port, uint16_t trig_pin);
uint8_t HCSR04_GetDis (void);
#endif /* INC_HC_SR04_H_ */
