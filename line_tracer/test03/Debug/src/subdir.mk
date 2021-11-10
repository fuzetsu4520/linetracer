################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cr_startup_lpc13xx.c \
../src/crp.c \
../src/ex_dock.c \
../src/kadai11_01_base2.c \
../src/pk_ltc.c \
../src/syscall.c \
../src/uart.c \
../src/uart_io.c 

OBJS += \
./src/cr_startup_lpc13xx.o \
./src/crp.o \
./src/ex_dock.o \
./src/kadai11_01_base2.o \
./src/pk_ltc.o \
./src/syscall.o \
./src/uart.o \
./src/uart_io.o 

C_DEPS += \
./src/cr_startup_lpc13xx.d \
./src/crp.d \
./src/ex_dock.d \
./src/kadai11_01_base2.d \
./src/pk_ltc.d \
./src/syscall.d \
./src/uart.d \
./src/uart_io.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC13xx -D__LPC13XX__ -D__REDLIB__ -I"C:\WorkSpace\ARM_C\test03\inc" -I"C:\WorkSpace\ARM_C\CMSIS_CORE_LPC13xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


