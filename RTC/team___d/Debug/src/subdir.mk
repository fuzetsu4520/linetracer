################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/buzzer.c \
../src/cr_startup_lpc13xx.c \
../src/crp.c \
../src/ex_dock.c \
../src/i2c.c \
../src/pk_ltc.c \
../src/sb1602b.c \
../src/syscall.c \
../src/uart.c \
../src/uart_io.c 

OBJS += \
./src/buzzer.o \
./src/cr_startup_lpc13xx.o \
./src/crp.o \
./src/ex_dock.o \
./src/i2c.o \
./src/pk_ltc.o \
./src/sb1602b.o \
./src/syscall.o \
./src/uart.o \
./src/uart_io.o 

C_DEPS += \
./src/buzzer.d \
./src/cr_startup_lpc13xx.d \
./src/crp.d \
./src/ex_dock.d \
./src/i2c.d \
./src/pk_ltc.d \
./src/sb1602b.d \
./src/syscall.d \
./src/uart.d \
./src/uart_io.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC13xx -D__LPC13XX__ -D__REDLIB__ -I"C:\WorkSpace\ARM_C\team___d\inc" -I"C:\WorkSpace\ARM_C\CMSIS_CORE_LPC13xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


