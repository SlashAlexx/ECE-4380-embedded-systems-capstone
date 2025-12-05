################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/grow_led.c \
../Core/Src/ina219.c \
../Core/Src/main.c \
../Core/Src/moisture_sensor.c \
../Core/Src/relay_pump.c \
../Core/Src/stm32l4xx_hal_msp.c \
../Core/Src/stm32l4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l4xx.c \
../Core/Src/uart_comms.c \
../Core/Src/uart_debug.c \
../Core/Src/ws28xx.c 

OBJS += \
./Core/Src/grow_led.o \
./Core/Src/ina219.o \
./Core/Src/main.o \
./Core/Src/moisture_sensor.o \
./Core/Src/relay_pump.o \
./Core/Src/stm32l4xx_hal_msp.o \
./Core/Src/stm32l4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l4xx.o \
./Core/Src/uart_comms.o \
./Core/Src/uart_debug.o \
./Core/Src/ws28xx.o 

C_DEPS += \
./Core/Src/grow_led.d \
./Core/Src/ina219.d \
./Core/Src/main.d \
./Core/Src/moisture_sensor.d \
./Core/Src/relay_pump.d \
./Core/Src/stm32l4xx_hal_msp.d \
./Core/Src/stm32l4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l4xx.d \
./Core/Src/uart_comms.d \
./Core/Src/uart_debug.d \
./Core/Src/ws28xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/grow_led.cyclo ./Core/Src/grow_led.d ./Core/Src/grow_led.o ./Core/Src/grow_led.su ./Core/Src/ina219.cyclo ./Core/Src/ina219.d ./Core/Src/ina219.o ./Core/Src/ina219.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/moisture_sensor.cyclo ./Core/Src/moisture_sensor.d ./Core/Src/moisture_sensor.o ./Core/Src/moisture_sensor.su ./Core/Src/relay_pump.cyclo ./Core/Src/relay_pump.d ./Core/Src/relay_pump.o ./Core/Src/relay_pump.su ./Core/Src/stm32l4xx_hal_msp.cyclo ./Core/Src/stm32l4xx_hal_msp.d ./Core/Src/stm32l4xx_hal_msp.o ./Core/Src/stm32l4xx_hal_msp.su ./Core/Src/stm32l4xx_it.cyclo ./Core/Src/stm32l4xx_it.d ./Core/Src/stm32l4xx_it.o ./Core/Src/stm32l4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l4xx.cyclo ./Core/Src/system_stm32l4xx.d ./Core/Src/system_stm32l4xx.o ./Core/Src/system_stm32l4xx.su ./Core/Src/uart_comms.cyclo ./Core/Src/uart_comms.d ./Core/Src/uart_comms.o ./Core/Src/uart_comms.su ./Core/Src/uart_debug.cyclo ./Core/Src/uart_debug.d ./Core/Src/uart_debug.o ./Core/Src/uart_debug.su ./Core/Src/ws28xx.cyclo ./Core/Src/ws28xx.d ./Core/Src/ws28xx.o ./Core/Src/ws28xx.su

.PHONY: clean-Core-2f-Src

