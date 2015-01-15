################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ParTest/ParTest.c 

OBJS += \
./ParTest/ParTest.o 


# Each subdirectory must supply rules for building sources it contributes
ParTest/%.o: ../ParTest/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 C Compiler'
	msp430-gcc -I/usr/msp430/include -I/opt/octanis1/FreeRTOSV8.1.2/FreeRTOS/Source/portable/GCC/MSP430F449 -I"/home/raffael/workspace/msptest" -I/opt/octanis1/FreeRTOSV8.1.2/FreeRTOS/Demo/Common/include -I/opt/octanis1/FreeRTOSV8.1.2/FreeRTOS/Source/include -O0 -g -Wall -mmcu=msp430f5529 -std=gnu89 -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


