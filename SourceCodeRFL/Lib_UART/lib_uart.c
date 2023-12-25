/*
 * lib_uart.c
 *
 *  Created on: Aug 30, 2023
 *      Author: BaoThuan
 */
#include"lib_uart.h"
/*extern variable*/
	float kx[3] = {0.0035, 0.0008, 0.0035};
/*static variable*/
//	LINE_STATE state_line_scheck = LINE_BLACK;

	static char All_sheet_TX[60] = {0}; 						// Mảng lưu chuỗi truy�?n đi có 2560 bits
	static UART_HandleTypeDef Lib_uart;
	static const char * confirmStart = "START";
	static const char * confirmStop = "STOP";
	static const char * confirmThreshold = "THRESHOLD";
	static const char * confirmStartMotor = "StartM";
	static const char * confirmPower = "Power";

	static const char * confirmKp = "Kp:";							//Kp: xx, Ki: yy, Kd: zz
	static const char * confirmKi = "Ki:";
	static const char * confirmKd = "Kd:";

	static const char * changeLine = "Line:";

	static const char * confirmBaseSpeedR = "BSR:";
	static const char * confirmBaseSpeedL = "BSL:";

	static const char * confirmMaxSpeedRight = "MSR:";
	static const char * confirmMaxSpeedLeft = "MSL:";

	static const char * confirm_motor_quick_right = "QR:";
	static const char * confirm_motor_quick_left = "QL:";

	STATE_ROBOT State_Robot 		= 	STOP_ROBOT;
/*Init uart used in lib_uart*/
	void bluetooth_init_UART(UART_HandleTypeDef *huart)
	{
		Lib_uart = *huart;
	}
/* Phản hồi theo form:
		//----------//
		Bắt đầu:
			Tốc độ động cơ trái = ?/100, Tốc độ động cơ phải = ?/100
			Thông số: Kp = ?, Ki = ?, Kd = ?
			Cảm biến : 0100 0000
			Dung lượng pin: ?/100
		//---------//
*/
	void bluetooth_Return_parameter(uint8_t state_tx_lv, uint8_t speed_debug_right, uint8_t speed_debug_left, float * Kx, uint8_t * sensor_read)
	{
		if(state_tx_lv == 0)
		{
			sprintf(All_sheet_TX,
						  "\n\nNL\nSLM:%03d,SRM:%03d\nKp=%1.4f, Ki=%1.4f, Kd=%1.3f\n",
						  speed_debug_left, speed_debug_right, Kx[0], Kx[1], Kx[2]);

			HAL_UART_Transmit(&Lib_uart, (uint8_t *)All_sheet_TX, 43, 1000);					// 90 * 4 = 320 bytes; 320bytes * 8 = 2560 bits
			memset(All_sheet_TX,0,90);
		}
																				//Kx: Kp = %1.4f, Ki = %1.4f, Kd = %2.4f\nSensor: %01u%01u%01u%01u%01u%01u%01u%01u\r\n
		else
		{
			sprintf(All_sheet_TX,
						  "\n\nNL\nSLM:%03d,SRM:%03d\nSS:%01d%01d%01d%01d%01d%01d%01d%01d",
						  speed_debug_left, speed_debug_right, *(sensor_read + 0), *(sensor_read + 1), *(sensor_read + 2), *(sensor_read + 3), *(sensor_read + 4), *(sensor_read + 5), *(sensor_read + 6), *(sensor_read + 7));
			HAL_UART_Transmit(&Lib_uart, (uint8_t *)All_sheet_TX, 26, 1000); 				// 90 * 4 = 320 bytes; 320bytes * 8 = 2560 bits
		}
	}
/*Check string input is stop or start*/
	STATE_ROBOT bluetooth_check_string_stop_start_robot(uint8_t * rxBuffer)
	{
	/*Control robot*/
		if(strstr((char*)rxBuffer, confirmStart) != NULL)
		{
			HAL_UART_Transmit(&Lib_uart, (uint8_t *)confirmStart, sizeof(confirmStart), 1000);
			State_Robot = START_ROBOT;
		}
		else if( strstr((char*)rxBuffer, confirmStop) != NULL)
		{
			HAL_UART_Transmit(&Lib_uart, (uint8_t *)confirmStop, sizeof(confirmStop), 1000);
			State_Robot = STOP_ROBOT;
		}
		else if(strstr((char*)rxBuffer, confirmThreshold) != NULL)
		{
			HAL_UART_Transmit(&Lib_uart, (uint8_t *)confirmThreshold, sizeof(confirmThreshold), 1000);
			State_Robot = CHECK_THRESHOLD;
		}
		else if(strstr((char*)rxBuffer, confirmStartMotor) != NULL)
		{
			HAL_UART_Transmit(&Lib_uart, (uint8_t *)confirmStartMotor, sizeof(confirmStartMotor), 1000);
			if(state_motor == 0)
			{
				state_motor = 1;
			}
			else
			{
				state_motor = 0;
			}
		}
		else if(strstr((char*)rxBuffer, confirmPower) != NULL)
		{
			HAL_UART_Transmit(&Lib_uart, (uint8_t *)confirmPower, sizeof(confirmPower), 1000);
			State_Robot = POWER;
		}
		return State_Robot;
	}
/*Chang Kx with value is recieved by phone*/
	void bluetooth_rx_change_kx(uint8_t *rxBuffer)
	{
		char * token = NULL;
		char string_feedback[13] = {0};
		/*kp: xx, ki: yy, kd: zz*/
		/*Set Kp*/
			if(strstr((char*)rxBuffer, confirmKp) != NULL)
			{
				token = strtok((char*)rxBuffer, ":");											//kp:
				token = strtok(NULL, ",");
				kx[0] = ((Char2Num(token) * 10) + Char2Num(token + 1)) / (float)10000;
				sprintf(string_feedback,"\nKp=%1.5f\n", kx[0]);
				HAL_UART_Transmit(&Lib_uart, (uint8_t *)string_feedback, sizeof(string_feedback), 1000);
			}
		/*Set Ki*/
			if(strstr((char*)rxBuffer, confirmKi) != NULL)
			{
				token = strtok((char*)rxBuffer, ":");											//kp: xx, ki:
				token = strtok(NULL, ",");
				kx[1] = ((Char2Num(token) * 10) + Char2Num(token + 1)) / (float)10000;
				sprintf(string_feedback,"\nKi=%1.5f\n", kx[1]);
				HAL_UART_Transmit(&Lib_uart, (uint8_t *)string_feedback, sizeof(string_feedback), 1000);
			}
		/*Set Kd*/
			if(strstr((char*)rxBuffer, confirmKd) != NULL)
			{
				token = strtok((char*)rxBuffer, ":");											//kp: xx, ki: yy, kd: zz
				token = strtok(NULL, ",");															//zz
				kx[2] = ((Char2Num(token) * 10) + Char2Num(token + 1)) / (float)10000;
//				sprintf(string_feedback,"\nKd=%1.4f\n", kx[3]);
//				kx[2] = ((Char2Num(token) * 10) + Char2Num(token + 1));
				sprintf(string_feedback,"\nKd=%03d\n", kx[3]);
				HAL_UART_Transmit(&Lib_uart, (uint8_t *)string_feedback, sizeof(string_feedback) - 1, 1000);
			}
		/*Set full speed*/
			if(strstr((char*)rxBuffer, confirmMaxSpeedRight) != NULL)
			{
				token = strtok((char*)rxBuffer, ":");
				token = strtok(NULL, ",");
				maxspeedr = (Char2Num(token) * 10) + Char2Num(token + 1);
				sprintf(string_feedback,"\nMSR=%03d\n", maxspeedl);
				HAL_UART_Transmit(&Lib_uart, (uint8_t *)string_feedback, sizeof(string_feedback)-4, 1000);
			}
			if(strstr((char*)rxBuffer, confirmMaxSpeedLeft) != NULL)
			{
				token = strtok((char*)rxBuffer, ":");
				token = strtok(NULL, ",");
				maxspeedl = (Char2Num(token) * 10) + Char2Num(token + 1);
				sprintf(string_feedback,"\nMSL=%03d\n", maxspeedl);
				HAL_UART_Transmit(&Lib_uart, (uint8_t *)string_feedback, sizeof(string_feedback)-4, 1000);
			}
		/*Set base speed*/
			if(strstr((char*)rxBuffer, confirmBaseSpeedR) != NULL)
			{
				token = strtok((char*)rxBuffer, ":");
				token = strtok(NULL, ",");
				basespeedr = (Char2Num(token) * 10) + Char2Num(token + 1);
				sprintf(string_feedback,"\nBSR=%03d", basespeedr);
				HAL_UART_Transmit(&Lib_uart, (uint8_t *)string_feedback, sizeof(string_feedback)-4, 1000);
			}
			if(strstr((char*)rxBuffer, confirmBaseSpeedL) != NULL)
			{
				token = strtok((char*)rxBuffer, ":");
				token = strtok(NULL, ",");
				basespeedl = (Char2Num(token) * 10) + Char2Num(token + 1);
				sprintf(string_feedback,"\nBSR=%03d", basespeedl);
				HAL_UART_Transmit(&Lib_uart, (uint8_t *)string_feedback, sizeof(string_feedback)-4, 1000);
			}
		/*Set quick speed Line*/
			if(strstr((char*)rxBuffer, confirm_motor_quick_right) != NULL)
			{
				token = strtok((char*)rxBuffer, ":");
				token = strtok(NULL, ",");
				quick_speed_right = (Char2Num(token) * 10) + Char2Num(token + 1);
				sprintf(string_feedback,"\nQSR=%03d", quick_speed_right);
				HAL_UART_Transmit(&Lib_uart, (uint8_t *)string_feedback, sizeof(string_feedback)-4, 1000);
			}
			if(strstr((char*)rxBuffer, confirm_motor_quick_left) != NULL)
			{
				token = strtok((char*)rxBuffer, ":");
				token = strtok(NULL, ",");
				quick_speed_left = (Char2Num(token) * 10) + Char2Num(token + 1);
				sprintf(string_feedback,"\nQSL=%03d", quick_speed_left);
				HAL_UART_Transmit(&Lib_uart, (uint8_t *)string_feedback, sizeof(string_feedback)-4, 1000);
			}
	}
/*func support for "bluetooth_rx_change_kx"*/
	uint8_t Char2Num(char * var_char)
	{
		uint8_t var_num = *var_char - 48;
		return var_num;
	}
/**/
	void change_line_sensor_read(uint8_t *rxBuffer)
	{
		char * token = NULL;
		char string_feedback[15] = {0};
		if(strstr((char*)rxBuffer, changeLine) != NULL)
		{
			token = strtok((char*)rxBuffer, ":");											//kp: xx, ki: yy, kd: zz
			token = strtok(NULL, ",");															//zz
			if(*token == 'W')
			{
				state_line_check = LINE_WHITE;
			}
			else if(*token == 'B')
			{
				state_line_check = LINE_BLACK;
			}
			sprintf(string_feedback,"\nLINE=%c", state_line_check);
			HAL_UART_Transmit(&Lib_uart, (uint8_t *)string_feedback, 7, 1000);
		}
	}
