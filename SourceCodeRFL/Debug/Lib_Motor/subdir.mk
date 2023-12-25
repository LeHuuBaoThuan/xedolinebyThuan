################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/STM_Folder/TrackingLine23.11.2023/N29-11-2023/Lib_Motor/lib_motor.c 

OBJS += \
./Lib_Motor/lib_motor.o 

C_DEPS += \
./Lib_Motor/lib_motor.d 


# Each subdirectory must supply rules for building sources it contributes
Lib_Motor/lib_motor.o: D:/STM_Folder/TrackingLine23.11.2023/N29-11-2023/Lib_Motor/lib_motor.c Lib_Motor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/STM_Folder/TrackingLine23.11.2023/N29-11-2023/Lib_UART" -I"D:/STM_Folder/TrackingLine23.11.2023/N29-11-2023/Lib_Controller" -I"D:/STM_Folder/TrackingLine23.11.2023/N29-11-2023/Lib_Motor" -I"D:/STM_Folder/TrackingLine23.11.2023/N29-11-2023/Lib_Sensor" -I"D:/STM_Folder/TrackingLine23.11.2023/N29-11-2023/lib_HC_SR04" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Lib_Motor

clean-Lib_Motor:
	-$(RM) ./Lib_Motor/lib_motor.cyclo ./Lib_Motor/lib_motor.d ./Lib_Motor/lib_motor.o ./Lib_Motor/lib_motor.su

.PHONY: clean-Lib_Motor

