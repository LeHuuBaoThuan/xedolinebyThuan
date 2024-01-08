/*
 * Lib_Sensor.c
 *
 *  Created on: Oct 5, 2023
 *      Author: BaoThuan
 */
#include<Lib_Sensor.h>
/*extern variable*/
	LINE_BREAKDOW last_end = END_MIDDLE;
	uint16_t last_idle = 0;
	uint8_t active = 0;
	uint8_t flat_define_threshold = 0;
	uint16_t *	ADC_get_rawValue_sensor;
	uint16_t threshold[7] ={};
/*adc init*/
	static ADC_HandleTypeDef 				hadc_lib;
/*74HC4051 VAL*/
	GPIO_TypeDef * 					IC74HC4051_GPIOx;
	uint16_t 						IC74HC4051_S0_Pin;
	uint16_t						IC74HC4051_S1_Pin;
	uint16_t 						IC74HC4051_S2_Pin;
/*74HC595 VAL*/
	/*74HC595 Port*/
	GPIO_TypeDef *					IC74HC595_ST_Port;
	GPIO_TypeDef * 					IC74hc595_DS_Port;
	GPIO_TypeDef * 					IC74HC595_Shift_Port;
/*74HC595 Pin*/
	uint16_t 						IC74hc595_Shift_Pin;
	uint16_t						IC74hc595_DS_Pin;
	uint16_t						IC74HC595_ST_Pin;
/*user static variable*/
	uint16_t pos_sensor 		= 	0x00;
	uint16_t old_pos_sensor 	= 	0x00;

	uint8_t 	err_line			= 	0;
	LINE_STATE_COLOR  state_line_check	=	LINE_WHITE;
	/*Pre variable*/
		static uint8_t 	pre_active					= 0;
		static uint16_t	pre_pos						= 0;
/*Init function of 74HC4051 and 74HC595*/
	/*74HC4051 INIT*/
		void IC74HC4051_Init(	ADC_HandleTypeDef handler, GPIO_TypeDef * GPIO, 									\
								uint16_t GPIO_Pin_S0, uint16_t GPIO_Pin_S1, uint16_t GPIO_Pin_S2)					\
		{
			hadc_lib 							= 	handler;
			IC74HC4051_GPIOx 					= 	GPIO;
			IC74HC4051_S0_Pin 					= 	GPIO_Pin_S0;
			IC74HC4051_S1_Pin 					= 	GPIO_Pin_S1;
			IC74HC4051_S2_Pin 					= 	GPIO_Pin_S2;
		}
	/*74HC595 INIT*/
		void IC74HC595_Init(	GPIO_TypeDef * DS_Port, GPIO_TypeDef * Shift_Port, GPIO_TypeDef * ST_Port, 			\
								uint16_t DS_Pin, uint16_t SH_Pin, uint16_t ST_Pin)									\
		{
			IC74HC595_ST_Port		=	ST_Port;
			IC74hc595_DS_Port		= 	DS_Port;
			IC74HC595_Shift_Port	=	Shift_Port;
			IC74HC595_ST_Pin		=	ST_Pin;
			IC74hc595_Shift_Pin		=	SH_Pin;
			IC74hc595_DS_Pin		=	DS_Pin;
		}
/*74HC4051 FUNCTION*/
	GPIO_PinState IC74hc4051_BitRead(uint8_t channel_01, uint8_t index)
	{
		if(((channel_01 >> index) & 1) == 0)
		{
			return GPIO_PIN_RESET;
		}
		else
		{
			return GPIO_PIN_SET;
		}
	}
	void IC74hc4051_SelectChannel(uint8_t channel)
	{
		HAL_GPIO_WritePin(IC74HC4051_GPIOx, IC74HC4051_S0_Pin | IC74HC4051_S1_Pin | IC74HC4051_S2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IC74HC4051_GPIOx, IC74HC4051_S0_Pin, IC74hc4051_BitRead(channel, 0));
		HAL_GPIO_WritePin(IC74HC4051_GPIOx, IC74HC4051_S1_Pin, IC74hc4051_BitRead(channel, 1));
		HAL_GPIO_WritePin(IC74HC4051_GPIOx, IC74HC4051_S2_Pin, IC74hc4051_BitRead(channel, 2));
	}
	/*Read real Value 1 Sensor*/
	uint16_t* IC74hc4051_Read_rawADC_1Chan(uint8_t i)
	{
		static uint16_t 		value1[8] 	= {0};
		IC74hc4051_SelectChannel(i);  													// select one mux pin at a time
		HAL_ADC_Start(&hadc_lib); 															// start the ADC
		HAL_ADC_PollForConversion(&hadc_lib, 100);  										// poll for conversion
		value1[i] = HAL_ADC_GetValue(&hadc_lib);  											// store adc value in value
		HAL_ADC_Stop(&hadc_lib);
		return value1;
	}
	/*Read real Value 7 Sensor*/
	uint16_t* IC74hc4051_Read_rawADC_7Chan()
	{
		uint8_t 				i			= 0;
		static uint16_t 		value[8] 	= {0};
		for(i = 0; i <= 7; i++)
		{
			IC74hc4051_SelectChannel(i);												// select one mux pin at a time
			HAL_ADC_Start(&hadc_lib); 														// start the ADC
			HAL_ADC_PollForConversion(&hadc_lib, 1000);  									// poll for conversion
			value[i] 	=  (HAL_ADC_GetValue(&hadc_lib) *100) / 4096;  						// store adc value in value, return value between 0 - 100
			HAL_ADC_Stop(&hadc_lib);
		}
		return value;
	}
/*74HC595 FUNCTION*/
	void IC74HC595(uint16_t  tt)
	{
		uint16_t	tam 	= 	0;
		uint8_t		i 		= 	0;
		tam 				= 	tt;
		for(i = 0; i < 8; i++)
		{
			if(tam & (0x80 >> i))
			{
				HAL_GPIO_WritePin(IC74hc595_DS_Port, IC74hc595_DS_Pin, GPIO_PIN_SET);
			}
			else
			{
				HAL_GPIO_WritePin(IC74hc595_DS_Port, IC74hc595_DS_Pin, GPIO_PIN_RESET);
			}
			HAL_GPIO_WritePin(IC74HC595_Shift_Port, IC74hc595_Shift_Pin ,GPIO_PIN_SET);
			HAL_Delay(1);
			HAL_GPIO_WritePin(IC74HC595_Shift_Port, IC74hc595_Shift_Pin, GPIO_PIN_RESET);
			HAL_Delay(1);
		}
		HAL_GPIO_WritePin(IC74HC595_ST_Port, IC74HC595_ST_Pin, GPIO_PIN_SET);
		HAL_Delay(1);
		HAL_GPIO_WritePin(IC74HC595_ST_Port, IC74HC595_ST_Pin, GPIO_PIN_RESET);
		HAL_Delay(1);
	}
/*Blink LED on Sensor*/
	uint16_t define_data(uint16_t* threshold)
	{
		ADC_get_rawValue_sensor 	= IC74hc4051_Read_rawADC_7Chan(); 				/*Extern variable*/
		/*Func variable*/
			uint16_t 	Led_Feedback 				= 0x00;
			uint16_t 	MATLED[7] 					= {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40}; // Cathode chung ( chân GND nối chung)
		/*VAL Support func PID*/
			uint16_t 	pos 						= 0;
						active	 					= 0;							/*Extern variable*/
//						last_end 					= END_MIDDLE;					/*Extern variable*/
		/*Check line: BLACK and change value: POS*/
			if (state_line_check == LINE_BLACK)
			{
				if (threshold[0] < ADC_get_rawValue_sensor[S0])
				{
					pos += 1000;
					Led_Feedback |= MATLED[0];									// Set BIT
					if((active == 0) && (last_end == END_LEFT))
					{
						__NOP();
					}
					else
					{
						last_end = END_RIGHT;
						active++;
					}
				}
				else
				{
					Led_Feedback &= ~MATLED[0];									// Clear BIT
				}
				if (threshold[6] < ADC_get_rawValue_sensor[S6])
				{
					pos += 7000;
					Led_Feedback |= MATLED[6];									// Set BIT
					if((active == 0) && (last_end == END_RIGHT))
					{
						__NOP();
					}
					else
					{
						last_end = END_LEFT;
						active++;
					}// Set BIT
				}
				else
				{
					Led_Feedback &= ~MATLED[6];									// Clear BIT
				}
				if (threshold[1] < ADC_get_rawValue_sensor[S1])
				{
					pos += 2000;
					Led_Feedback |= MATLED[1];									// Set BIT
//					last_end = END_RIGHT;
					if((active == 0) && (last_end == END_LEFT))
					{
						__NOP();
					}
					else
					{
						last_end 					= END_MIDDLE;
						active++;
					}
				}
				else
				{
					Led_Feedback &= ~MATLED[1];									// Clear BIT
				}
				if (threshold[2] < ADC_get_rawValue_sensor[S2])
				{
					pos += 3000;
					active++;
					Led_Feedback |= MATLED[2];
					last_end 					= END_MIDDLE;// Set BIT
				}
				else
				{
					Led_Feedback &= ~MATLED[2];									// Clear BIT
				}
				if (threshold[3] < ADC_get_rawValue_sensor[S3])
				{
					pos += 4000;
					active++;
					Led_Feedback |= MATLED[3];
					last_end 					= END_MIDDLE;// Set BIT
				}
				else
				{
					Led_Feedback &= ~MATLED[3];									// Clear BIT
				}
				if (threshold[4] < ADC_get_rawValue_sensor[S4])
				{
					pos += 5000;
					active++;
					Led_Feedback |= MATLED[4];
					last_end 					= END_MIDDLE;// Set BIT
				}
				else
				{
					Led_Feedback &= ~MATLED[4];									// Clear BIT
				}
				if (threshold[5] < ADC_get_rawValue_sensor[S5])
				{
					pos += 6000;
					Led_Feedback |= MATLED[5];									// Set BIT
//					last_end = END_LEFT;
					if((active == 0) && (last_end == END_RIGHT))
					{
						__NOP();
					}
					else
					{
						last_end 					= END_MIDDLE;
						active++;
					}
				}
				else
				{
					Led_Feedback &= ~MATLED[5];									// Clear BIT
				}
			}
		/*End check line: BLACK*/

		/*Check line: WHITE and change value: POS*/
			else if(state_line_check == LINE_WHITE)
			{
				if (threshold[0] >= ADC_get_rawValue_sensor[S0])
				{
					pos += 1000;
					Led_Feedback |= MATLED[0];									// Set BIT
					if((active == 0) && (last_end == END_LEFT))
					{
						__NOP();
					}
					else
					{
						last_end = END_RIGHT;
						active++;
					}
				}
				else
				{
					Led_Feedback &= ~MATLED[0];									// Clear BIT
				}
				if (threshold[6] >= ADC_get_rawValue_sensor[S6])
				{
					pos += 7000;
					Led_Feedback |= MATLED[6];
					if((active == 0) && (last_end == END_RIGHT))
					{
						__NOP();
					}
					else
					{
						last_end = END_LEFT;
						active++;
					}// Set BIT
				}
				else
				{
					Led_Feedback &= ~MATLED[6];									// Clear BIT
				}
				if (threshold[1] >= ADC_get_rawValue_sensor[S1])
				{
					pos += 2000;
					active++;
					Led_Feedback |= MATLED[1];									// Set BIT
//					last_end = END_RIGHT;
					last_end 					= END_MIDDLE;
				}
				else
				{
					Led_Feedback &= ~MATLED[1];									// Clear BIT
				}
				if (threshold[2] >= ADC_get_rawValue_sensor[S2])
				{
					pos += 3000;
					active++;
					Led_Feedback |= MATLED[2];									// Set BIT
					last_end 					= END_MIDDLE;
				}
				else
				{
					Led_Feedback &= ~MATLED[2];									// Clear BIT
				}
				if (threshold[3] >= ADC_get_rawValue_sensor[S3])
				{
					pos += 4000;
					active++;
					Led_Feedback |= MATLED[3];
					last_end 					= END_MIDDLE;// Set BIT
				}
				else
				{
					Led_Feedback &= ~MATLED[3];									// Clear BIT
				}
				if (threshold[4] >= ADC_get_rawValue_sensor[S4])
				{
					pos += 5000;
					active++;
					Led_Feedback |= MATLED[4];									// Set BIT
					last_end 					= END_MIDDLE;
				}
				else
				{
					Led_Feedback &= ~MATLED[4];									// Clear BIT
				}
				if (threshold[5] >= ADC_get_rawValue_sensor[S5])
				{
					pos += 6000;
					active++;
					Led_Feedback |= MATLED[5];									// Set BIT
//					last_end = END_LEFT;
					last_end 					= END_MIDDLE;
				}
				else
				{
					Led_Feedback &= ~MATLED[5];									// Clear BIT
				}
			}
		/* End check line: WHITE*/
		/* Set LED on Sensor circuit */
			IC74HC595(Led_Feedback);
		/*Check sensor: Giai quyet vach xuong ca*/
			pos_sensor = Led_Feedback;
			if		(Check_error(pos_sensor) == LINE_ERROR)								// If haven't right state
			{
				/*Tam dung doc sensor va cho chay theo vi tri cu~*/
					active 						= pre_active;
					pos							= pre_pos;
					last_end 					= END_MIDDLE;
			}
			else if	(Check_error(pos_sensor) == LINE_VALID)
			{
					pre_active					= active;
					pre_pos						= pos;
			}
		/*Return value*/
			if (active == 0)
			{
				HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
				last_idle ++;
				return 0;
			}
			else
			{
				last_idle = 0;
				return pos/(uint16_t)active;
			}
	}
	/*Line signal error??*/
	LINE_STATE Check_error(uint16_t F_pos_SS)
	{
		uint16_t 	pos1_valid[7]	=	{0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40};
		uint16_t 	pos2_valid[6]	=	{0x60, 0x18, 0x06, 0x30, 0x0C, 0x03};
		uint16_t	pos3_valid[5]	=	{0x38, 0x07, 0x70, 0x0D, 0x1C};
//		uint16_t 	pos4_valid[4]	= 	{0xF0, 0x0F, 0x1E, 0x3C};
//		uint16_t 	pos5_valid[2]	= 	{0x7C, 0x1F};

		uint16_t	tam 			= 	F_pos_SS;
		uint8_t 	count			=	0;
		uint8_t		valid_mem		=	0;
		/*How many SS get line = count*/
			for(uint8_t i = 1; i <= 7; i++)
			{
				if(tam & (0x80 >> i))
				{
					count++;
				}
			}
		/*State Line BLACK LINE*/
			/*STate LINE BLACK = 1*/
			if(count == 1)
			{
				for(uint8_t i1 = 0; i1 <= sizeof(pos1_valid); i1++)
				{
					if(tam == pos1_valid[i1])
					{
						valid_mem++;
					}
				}
			}
			/*STate LINE BLACK = 2*/
			else if(count == 2)
			{
				for(uint8_t i2 = 0; i2 <= sizeof(pos2_valid); i2++)
				{
					if(tam == pos2_valid[i2])
					{
						valid_mem++;
					}
				}
			}
			/*STate LINE BLACK = 3*/
			else if(count == 3)
			{
				for(uint8_t i3 = 0; i3 <= sizeof(pos3_valid); i3++)
				{
					if(tam == pos3_valid[i3])
					{
						valid_mem++;
					}
				}
			}
			/*STate LINE BLACK = 4*/
//			else if(count == 4)
//			{
//				for(uint8_t i4 = 0; i4 <= sizeof(pos4_valid); i4++)
//				{
//					if(tam == pos4_valid[i4])
//					{
//						valid_mem++;
//					}
//				}
//			}
//			/*STate LINE BLACK = 5*/
//			else if(count == 5)
//			{
//				for(uint8_t i5 = 0; i5 <= sizeof(pos5_valid); i5++)
//				{
//					if(tam == pos5_valid[i5])
//					{
//						valid_mem++;
//					}
//				}
//			}

		if(valid_mem >= 1 || count == 0)
		{
			return LINE_VALID;
		}
		else
		{
			return LINE_ERROR;
		}
	}
/*Sensor define threshold*/
//		uint16_t * define_threshold(uint16_t* ADC_get_rawValue_sensor)
//		{
//			uint16_t threshold_L 	[7] = {0};
//			uint16_t threshold_H 	[7] = {0};
//			static uint16_t threshold[7];
//			if(flat_define_threshold == 1)
//			{
//				//Low Threshold: bat dau doc gia tri vach den/trang
//					threshold_L[0]=ADC_get_rawValue_sensor[0];
//					threshold_L[1]=ADC_get_rawValue_sensor[1];
//					threshold_L[2]=ADC_get_rawValue_sensor[2];
//					threshold_L[3]=ADC_get_rawValue_sensor[3];
//					threshold_L[4]=ADC_get_rawValue_sensor[4];
//					threshold_L[5]=ADC_get_rawValue_sensor[5];
//					threshold_L[6]=ADC_get_rawValue_sensor[6];
//				// Nhap nhay led
//					blink_led(5);
//			}
//			if(flat_define_threshold == 2)
//			{
//				//High Threshold: bat dau doc gia tri vach con lai
//					threshold_H[0]=ADC_get_rawValue_sensor[0];
//					threshold_H[1]=ADC_get_rawValue_sensor[1];
//					threshold_H[2]=ADC_get_rawValue_sensor[2];
//					threshold_H[3]=ADC_get_rawValue_sensor[3];
//					threshold_H[4]=ADC_get_rawValue_sensor[4];
//					threshold_H[5]=ADC_get_rawValue_sensor[5];
//					threshold_H[6]=ADC_get_rawValue_sensor[6];
//				// Nhap nhay led
//					blink_led(5);
//				flat_define_threshold = 0;
//				for(uint8_t k=0; k <= 6; k++)
//				{
//					threshold[k] = (threshold_H[k] + threshold_L[k])/2;
//				}
//			}
//			return threshold;
//		}
		void blink_led(uint8_t tt)
		{
			uint16_t blink_all_led = 0xFF;
			for(uint8_t i = 0; i <= tt; i++)
			{
				IC74HC595(blink_all_led);
				HAL_Delay(100);
				IC74HC595(blink_all_led & 0x00);
				HAL_Delay(500);
			}
		}
//		uint16_t * action_define_threshold()
//		{
//			ADC_get_rawValue_sensor = 	IC74hc4051_Read_rawADC_7Chan();
//
//			uint8_t 	count 					= 	0;
//			uint32_t time_begin = 0;
//			uint32_t time_now = 0;
//			flat_define_threshold 				= 	1;
//			threshold 							= 	define_threshold(ADC_get_rawValue_sensor);
//			motor_move(50);
//			HAL_Delay(5000);
//			time_begin = HAL_GetTick();
//			while(1)
//			{
//				ADC_get_rawValue_sensor 	= 	IC74hc4051_Read_rawADC_7Chan();
//				for(uint8_t i = 0; i <= 6; i++)
//				{
//					if(ADC_get_rawValue_sensor[i] <= 20)
//					{
//						count++;
//					}
//				}
//				if(count == 7)
//				{
//					flat_define_threshold 	= 	2;
//					threshold 				= 	define_threshold(ADC_get_rawValue_sensor);
//					count 					= 	0;
//					motor_down(50, time_now - time_begin);
//					break;
//				}
//				else
//				{
//					count 					= 	0;
//					// 5s tu thoat while 1
//				}
//				/*Out loop when car move 5s*/
//				time_now = HAL_GetTick();
//				if(time_now - time_begin >= 5000)
//				{
//					motor_down(50, time_now - time_begin);
//					break;
//				}
//			}
//			return threshold;
//		}
