/*
 * Lib_Controller.c
 *
 *  Created on: Oct 23, 2023
 *      Author: BaoThuan
 */
#include<Lib_Controller.h>

/*Extern variable*/
/*Dong Co Trai nhanh hon dong co Phai*/
uint8_t basespeedr = 37;
uint8_t basespeedl = 35;

uint8_t maxspeedr = 75;
uint8_t maxspeedl = 70;
/*Debug Speed*/
int speed_right_debug = 0;
int speed_left_debug = 0;
/*Static variable*/
static int errors[10] = { 0 };
static int lastError = 0;
/*Supports PID algorithm*/
void past_errors(int error) {
	for (int i = 9; i > 0; i--) {
		errors[i] = errors[i - 1];
	}
	errors[0] = error;
}
int errors_sum(int index, int abs) {
	int sum = 0;
	for (int i = 0; i < index; i++) {
		if (abs == 1 && errors[i] < 0)
			sum += -errors[i];
		else
			sum += errors[i];
	}
	return sum;
}
/*PID Control*/
void PID_control(float *Kx, uint16_t *threshold_func_PID)	// Kx: Kp, Ki, Kd
{
	int position = define_data(threshold_func_PID);
	int error = 4000 - position;
	int P = 0;
	int I = 0;
	int D = 0;

	past_errors(error);
	// Kx: Kp, Ki, Kd
	P = error * Kx[0];
	I = errors_sum(5, 0) * Kx[1];
	D = (error - lastError) * Kx[2];

	lastError = error;

	int motorspeed = P + I + D;

	int motor_speed_right = basespeedr - motorspeed;
	int motor_speed_left = basespeedl + motorspeed;
	/*Feed rate*/
	if (motor_speed_right > maxspeedl) {
		motor_speed_right = maxspeedl;
	}
	if (motor_speed_left > maxspeedr) {
		motor_speed_left = maxspeedr;
	}
	/*Control motor*/
	forward_brake(motor_speed_right, motor_speed_left);

}
/*Ne_vat_can___Cua_phai*/
void Ne_vat_can___Cua_phai(uint8_t speed_Right, uint8_t speed_Left,
		uint32_t Time_Move, uint32_t Time_Right, uint32_t Time_Left) {
//		ADC_get_rawValue_sensor 	= IC74hc4051_Read_rawADC_7Chan();
	static uint16_t *ss3 = 0;
	motor_control(-20, -20);
	HAL_Delay(500);

	motor_control(0, speed_Left);
	HAL_Delay(Time_Right);

	motor_control(40, 35);
	HAL_Delay(Time_Move);

	motor_control(speed_Right, 0);
	HAL_Delay(Time_Left);

	ss3 = IC74hc4051_Read_rawADC_1Chan(3);
	while (ss3[3] >= 2800) {
		motor_control(40, 35);
		ss3 = IC74hc4051_Read_rawADC_1Chan(3);
	}
	motor_control(-20, -20);
	HAL_Delay(500);
	motor_control(0, 0);
}
