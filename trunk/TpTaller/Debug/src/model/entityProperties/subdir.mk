################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/model/entityProperties/Base.cpp \
../src/model/entityProperties/Coordinates.cpp \
../src/model/entityProperties/Position.cpp \
../src/model/entityProperties/Power.cpp \
../src/model/entityProperties/Speed.cpp 

OBJS += \
./src/model/entityProperties/Base.o \
./src/model/entityProperties/Coordinates.o \
./src/model/entityProperties/Position.o \
./src/model/entityProperties/Power.o \
./src/model/entityProperties/Speed.o 

CPP_DEPS += \
./src/model/entityProperties/Base.d \
./src/model/entityProperties/Coordinates.d \
./src/model/entityProperties/Position.d \
./src/model/entityProperties/Power.d \
./src/model/entityProperties/Speed.d 


# Each subdirectory must supply rules for building sources it contributes
src/model/entityProperties/%.o: ../src/model/entityProperties/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -I/usr/local/include/yaml-cpp -I"/home/tomas/workspace/TpTaller/includes" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


