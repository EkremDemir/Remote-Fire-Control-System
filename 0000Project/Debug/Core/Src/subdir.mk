################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ESP_Handle.c \
../Core/Src/Helper_Func.c \
../Core/Src/RingBuffer.c \
../Core/Src/ServoMotor.c \
../Core/Src/dcMotor.c \
../Core/Src/it.c \
../Core/Src/main.c \
../Core/Src/msp.c \
../Core/Src/stepMotor.c \
../Core/Src/system_stm32f4xx.c 

OBJS += \
./Core/Src/ESP_Handle.o \
./Core/Src/Helper_Func.o \
./Core/Src/RingBuffer.o \
./Core/Src/ServoMotor.o \
./Core/Src/dcMotor.o \
./Core/Src/it.o \
./Core/Src/main.o \
./Core/Src/msp.o \
./Core/Src/stepMotor.o \
./Core/Src/system_stm32f4xx.o 

C_DEPS += \
./Core/Src/ESP_Handle.d \
./Core/Src/Helper_Func.d \
./Core/Src/RingBuffer.d \
./Core/Src/ServoMotor.d \
./Core/Src/dcMotor.d \
./Core/Src/it.d \
./Core/Src/main.d \
./Core/Src/msp.d \
./Core/Src/stepMotor.d \
./Core/Src/system_stm32f4xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/ESP_Handle.d ./Core/Src/ESP_Handle.o ./Core/Src/ESP_Handle.su ./Core/Src/Helper_Func.d ./Core/Src/Helper_Func.o ./Core/Src/Helper_Func.su ./Core/Src/RingBuffer.d ./Core/Src/RingBuffer.o ./Core/Src/RingBuffer.su ./Core/Src/ServoMotor.d ./Core/Src/ServoMotor.o ./Core/Src/ServoMotor.su ./Core/Src/dcMotor.d ./Core/Src/dcMotor.o ./Core/Src/dcMotor.su ./Core/Src/it.d ./Core/Src/it.o ./Core/Src/it.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/msp.d ./Core/Src/msp.o ./Core/Src/msp.su ./Core/Src/stepMotor.d ./Core/Src/stepMotor.o ./Core/Src/stepMotor.su ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su

.PHONY: clean-Core-2f-Src

