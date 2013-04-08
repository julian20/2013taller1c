################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/model/persistence/ConfigurationReader.cpp \
../src/model/persistence/PersistentConfiguration.cpp 

OBJS += \
./src/model/persistence/ConfigurationReader.o \
./src/model/persistence/PersistentConfiguration.o 

CPP_DEPS += \
./src/model/persistence/ConfigurationReader.d \
./src/model/persistence/PersistentConfiguration.d 


# Each subdirectory must supply rules for building sources it contributes
src/model/persistence/%.o: ../src/model/persistence/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -I/usr/local/include/yaml-cpp -I"/home/tomas/workspace/TpTaller/includes" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


