################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/DHT11.c \
../Core/Src/WASHING_MACHINE_PROJ.c \
../Core/Src/button.c \
../Core/Src/buzzer.c \
../Core/Src/dcmotor.c \
../Core/Src/fnd4dight.c \
../Core/Src/i2c_lcd.c \
../Core/Src/internal_rtc.c \
../Core/Src/led.c \
../Core/Src/main.c \
../Core/Src/servo_motor.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/uart.c \
../Core/Src/ultrasonic.c 

OBJS += \
./Core/Src/DHT11.o \
./Core/Src/WASHING_MACHINE_PROJ.o \
./Core/Src/button.o \
./Core/Src/buzzer.o \
./Core/Src/dcmotor.o \
./Core/Src/fnd4dight.o \
./Core/Src/i2c_lcd.o \
./Core/Src/internal_rtc.o \
./Core/Src/led.o \
./Core/Src/main.o \
./Core/Src/servo_motor.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/uart.o \
./Core/Src/ultrasonic.o 

C_DEPS += \
./Core/Src/DHT11.d \
./Core/Src/WASHING_MACHINE_PROJ.d \
./Core/Src/button.d \
./Core/Src/buzzer.d \
./Core/Src/dcmotor.d \
./Core/Src/fnd4dight.d \
./Core/Src/i2c_lcd.d \
./Core/Src/internal_rtc.d \
./Core/Src/led.d \
./Core/Src/main.d \
./Core/Src/servo_motor.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/uart.d \
./Core/Src/ultrasonic.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F429xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/DHT11.cyclo ./Core/Src/DHT11.d ./Core/Src/DHT11.o ./Core/Src/DHT11.su ./Core/Src/WASHING_MACHINE_PROJ.cyclo ./Core/Src/WASHING_MACHINE_PROJ.d ./Core/Src/WASHING_MACHINE_PROJ.o ./Core/Src/WASHING_MACHINE_PROJ.su ./Core/Src/button.cyclo ./Core/Src/button.d ./Core/Src/button.o ./Core/Src/button.su ./Core/Src/buzzer.cyclo ./Core/Src/buzzer.d ./Core/Src/buzzer.o ./Core/Src/buzzer.su ./Core/Src/dcmotor.cyclo ./Core/Src/dcmotor.d ./Core/Src/dcmotor.o ./Core/Src/dcmotor.su ./Core/Src/fnd4dight.cyclo ./Core/Src/fnd4dight.d ./Core/Src/fnd4dight.o ./Core/Src/fnd4dight.su ./Core/Src/i2c_lcd.cyclo ./Core/Src/i2c_lcd.d ./Core/Src/i2c_lcd.o ./Core/Src/i2c_lcd.su ./Core/Src/internal_rtc.cyclo ./Core/Src/internal_rtc.d ./Core/Src/internal_rtc.o ./Core/Src/internal_rtc.su ./Core/Src/led.cyclo ./Core/Src/led.d ./Core/Src/led.o ./Core/Src/led.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/servo_motor.cyclo ./Core/Src/servo_motor.d ./Core/Src/servo_motor.o ./Core/Src/servo_motor.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/uart.cyclo ./Core/Src/uart.d ./Core/Src/uart.o ./Core/Src/uart.su ./Core/Src/ultrasonic.cyclo ./Core/Src/ultrasonic.d ./Core/Src/ultrasonic.o ./Core/Src/ultrasonic.su

.PHONY: clean-Core-2f-Src

