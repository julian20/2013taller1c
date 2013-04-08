################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/view/map/EntityViewMap.cpp \
../src/view/map/MapCameraView.cpp \
../src/view/map/MapView.cpp 

OBJS += \
./src/view/map/EntityViewMap.o \
./src/view/map/MapCameraView.o \
./src/view/map/MapView.o 

CPP_DEPS += \
./src/view/map/EntityViewMap.d \
./src/view/map/MapCameraView.d \
./src/view/map/MapView.d 


# Each subdirectory must supply rules for building sources it contributes
src/view/map/%.o: ../src/view/map/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include -I/usr/local/include/yaml-cpp -I"/home/tomas/workspace/TpTaller/includes" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


