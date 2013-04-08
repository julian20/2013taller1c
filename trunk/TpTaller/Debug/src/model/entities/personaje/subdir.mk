################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/model/entities/personaje/Personaje.cpp 

OBJS += \
./src/model/entities/personaje/Personaje.o 

CPP_DEPS += \
./src/model/entities/personaje/Personaje.d 


# Each subdirectory must supply rules for building sources it contributes
src/model/entities/personaje/%.o: ../src/model/entities/personaje/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -I/usr/local/include/yaml-cpp -I"/home/tomas/workspace/TpTaller/includes" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


