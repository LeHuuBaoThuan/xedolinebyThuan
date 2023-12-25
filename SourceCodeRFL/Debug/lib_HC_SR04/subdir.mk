################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/STM_Folder/TrackingLine23.11.2023/N29-11-2023/lib_HC_SR04/HC_SR04.c 

OBJS += \
./lib_HC_SR04/HC_SR04.o 

C_DEPS += \
./lib_HC_SR04/HC_SR04.d 


# Each subdirectory must supply rules for building sources it contributes
lib_HC_SR04/HC_SR04.o: D:/STM_Folder/TrackingLine23.11.2023/N29-11-2023/lib_HC_SR04/HC_SR04.c lib_HC_SR04/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"D:/STM_Folder/TrackingLine23.11.2023/N29-11-2023/Lib_UART" -I"D:/STM_Folder/TrackingLine23.11.2023/N29-11-2023/Lib_Controller" -I"D:/STM_Folder/TrackingLine23.11.2023/N29-11-2023/Lib_Motor" -I"D:/STM_Folder/TrackingLine23.11.2023/N29-11-2023/Lib_Sensor" -I"D:/STM_Folder/TrackingLine23.11.2023/N29-11-2023/lib_HC_SR04" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-lib_HC_SR04

clean-lib_HC_SR04:
	-$(RM) ./lib_HC_SR04/HC_SR04.cyclo ./lib_HC_SR04/HC_SR04.d ./lib_HC_SR04/HC_SR04.o ./lib_HC_SR04/HC_SR04.su

.PHONY: clean-lib_HC_SR04

