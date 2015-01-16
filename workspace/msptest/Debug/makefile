################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include serial/subdir.mk
-include ParTest/subdir.mk
-include subdir.mk
-include objects.mk

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
ARCHIVES += \
libUSER_OBJS.a \


# All Target
all: msptest.elf

# Tool invocations
msptest.elf: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MSP430 Linker'
	/usr/local/msp430-toolchain/bin/msp430-gcc-wrapper $(OBJS) -mmcu=msp430f5529 -L/usr/msp430/lib -L/opt/octanis1 -o "msptest.elf"
	@echo 'Finished building target: $@'
	@echo ' '

libUSER_OBJS.a: $(OBJS) $(USER_OBJS)
	@echo 'Invoking: MSP430 Archiver'
	/usr/local/msp430-toolchain/bin/msp430-ar-wrapper rcs  "libUSER_OBJS.a" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ELFS)$(OBJS)$(ARCHIVES) msptest.elf
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
