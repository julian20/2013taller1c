################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/view/entities/personaje/PersonajeVista.cpp 

OBJS += \
./src/view/entities/personaje/PersonajeVista.o 

CPP_DEPS += \
./src/view/entities/personaje/PersonajeVista.d 


# Each subdirectory must supply rules for building sources it contributes
src/view/entities/personaje/%.o: ../src/view/entities/personaje/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -I/usr/local/include/yaml-cpp -I"/home/tomas/workspace/TpTaller/includes" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


