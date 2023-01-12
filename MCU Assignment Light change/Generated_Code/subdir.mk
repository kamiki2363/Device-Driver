################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Generated_Code/BLUE_LED.c \
../Generated_Code/Cpu.c \
../Generated_Code/GREEN_LED.c \
../Generated_Code/PE_LDD.c \
../Generated_Code/RED_LED.c \
../Generated_Code/TSS1.c \
../Generated_Code/TU1.c \
../Generated_Code/TU2.c \
../Generated_Code/Vectors.c 

OBJS += \
./Generated_Code/BLUE_LED.o \
./Generated_Code/Cpu.o \
./Generated_Code/GREEN_LED.o \
./Generated_Code/PE_LDD.o \
./Generated_Code/RED_LED.o \
./Generated_Code/TSS1.o \
./Generated_Code/TU1.o \
./Generated_Code/TU2.o \
./Generated_Code/Vectors.o 

C_DEPS += \
./Generated_Code/BLUE_LED.d \
./Generated_Code/Cpu.d \
./Generated_Code/GREEN_LED.d \
./Generated_Code/PE_LDD.d \
./Generated_Code/RED_LED.d \
./Generated_Code/TSS1.d \
./Generated_Code/TU1.d \
./Generated_Code/TU2.d \
./Generated_Code/Vectors.d 


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/%.o: ../Generated_Code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"E:/EEE226/Assesment_1/Static_Code/PDD" -I"E:/EEE226/Assesment_1/Static_Code/IO_Map" -I"E:/EEE226/Assesment_1/Sources" -I"E:/EEE226/Assesment_1/Generated_Code" -I"E:/EEE226/Assesment_1/Sources/TSS" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


