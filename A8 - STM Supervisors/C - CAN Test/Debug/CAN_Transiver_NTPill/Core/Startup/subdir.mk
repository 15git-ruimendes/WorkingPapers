################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../CAN_Transiver_NTPill/Core/Startup/startup_stm32f412retx.s 

OBJS += \
./CAN_Transiver_NTPill/Core/Startup/startup_stm32f412retx.o 

S_DEPS += \
./CAN_Transiver_NTPill/Core/Startup/startup_stm32f412retx.d 


# Each subdirectory must supply rules for building sources it contributes
CAN_Transiver_NTPill/Core/Startup/%.o: ../CAN_Transiver_NTPill/Core/Startup/%.s CAN_Transiver_NTPill/Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-CAN_Transiver_NTPill-2f-Core-2f-Startup

clean-CAN_Transiver_NTPill-2f-Core-2f-Startup:
	-$(RM) ./CAN_Transiver_NTPill/Core/Startup/startup_stm32f412retx.d ./CAN_Transiver_NTPill/Core/Startup/startup_stm32f412retx.o

.PHONY: clean-CAN_Transiver_NTPill-2f-Core-2f-Startup
