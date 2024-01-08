################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Lib_RFL/Lib_Motor/lib_motor.c 

OBJS += \
./Drivers/Lib_RFL/Lib_Motor/lib_motor.o 

C_DEPS += \
./Drivers/Lib_RFL/Lib_Motor/lib_motor.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Lib_RFL/Lib_Motor/%.o Drivers/Lib_RFL/Lib_Motor/%.su Drivers/Lib_RFL/Lib_Motor/%.cyclo: ../Drivers/Lib_RFL/Lib_Motor/%.c Drivers/Lib_RFL/Lib_Motor/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/STM_Folder/TrackingLine23.11.2023/N29-11-2023/Lib_Sensor" -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-Lib_RFL-2f-Lib_Motor

clean-Drivers-2f-Lib_RFL-2f-Lib_Motor:
	-$(RM) ./Drivers/Lib_RFL/Lib_Motor/lib_motor.cyclo ./Drivers/Lib_RFL/Lib_Motor/lib_motor.d ./Drivers/Lib_RFL/Lib_Motor/lib_motor.o ./Drivers/Lib_RFL/Lib_Motor/lib_motor.su

.PHONY: clean-Drivers-2f-Lib_RFL-2f-Lib_Motor

