################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/raffael/Desktop/Octanis/Octanis1-Eclipse/lib/octanis1/FreeRTOSV8.1.2/FreeRTOS/Source/portable/GCC/MSP430F449/port.c 

OBJS += \
./Source/portable/GCC/MSP430F449/port.o 


# Each subdirectory must supply rules for building sources it contributes
Source/portable/GCC/MSP430F449/port.o: /Users/raffael/Desktop/Octanis/Octanis1-Eclipse/lib/octanis1/FreeRTOSV8.1.2/FreeRTOS/Source/portable/GCC/MSP430F449/port.c
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 C Compiler'
	/usr/local/msp430-toolchain/bin/msp430-gcc-wrapper -I/usr/msp430/include -I/opt/octanis1/FreeRTOSV8.1.2/FreeRTOS/Source/portable/GCC/MSP430F449 -I"/Users/raffael/Desktop/Octanis/Octanis1-Eclipse/workspace/msptest" -I/opt/octanis1/FreeRTOSV8.1.2/FreeRTOS/Demo/Common/include -I/opt/octanis1/FreeRTOSV8.1.2/FreeRTOS/Source/include -O0 -g -Wall -mmcu=msp430f5529 -std=gnu89 -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


