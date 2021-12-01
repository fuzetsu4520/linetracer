################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/team_d/yyyyy.c 

OBJS += \
./src/team_d/yyyyy.o 

C_DEPS += \
./src/team_d/yyyyy.d 


# Each subdirectory must supply rules for building sources it contributes
src/team_d/%.o: ../src/team_d/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSIS_CORE_LPC13xx -D__LPC13XX__ -D__REDLIB__ -I"C:\WorkSpace\ARM_C\team___d\inc" -I"C:\WorkSpace\ARM_C\CMSIS_CORE_LPC13xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


