################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/view/map/subdir.mk
-include src/view/entities/personaje/subdir.mk
-include src/view/entities/subdir.mk
-include src/view/configuration/subdir.mk
-include src/view/subdir.mk
-include src/model/persistence/subdir.mk
-include src/model/map/subdir.mk
-include src/model/entityProperties/subdir.mk
-include src/model/entities/personaje/subdir.mk
-include src/model/entities/subdir.mk
-include src/model/Logs/subdir.mk
-include src/model/subdir.mk
-include src/controller/subdir.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: TpTaller

# Tool invocations
TpTaller: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MinGW C++ Linker'
	g++  -o "TpTaller" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) TpTaller
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
