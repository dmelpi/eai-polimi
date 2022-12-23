################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/pre_processing_library/Src/pre_processing_app.c \
../Core/pre_processing_library/Src/pre_processing_core.c 

OBJS += \
./Core/pre_processing_library/Src/pre_processing_app.o \
./Core/pre_processing_library/Src/pre_processing_core.o 

C_DEPS += \
./Core/pre_processing_library/Src/pre_processing_app.d \
./Core/pre_processing_library/Src/pre_processing_core.d 


# Each subdirectory must supply rules for building sources it contributes
Core/pre_processing_library/Src/%.o Core/pre_processing_library/Src/%.su: ../Core/pre_processing_library/Src/%.c Core/pre_processing_library/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U585xx -c -I../X-CUBE-AI/App -I../Middlewares/ST/AI/Inc -I../Core/Inc -I../Core/pre_processing_library/Inc -I../Drivers/BSP/STWIN.box -I../Drivers/BSP/Components/ism330dhcx -I../Drivers/BSP/Components/Common -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/DSP/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-pre_processing_library-2f-Src

clean-Core-2f-pre_processing_library-2f-Src:
	-$(RM) ./Core/pre_processing_library/Src/pre_processing_app.d ./Core/pre_processing_library/Src/pre_processing_app.o ./Core/pre_processing_library/Src/pre_processing_app.su ./Core/pre_processing_library/Src/pre_processing_core.d ./Core/pre_processing_library/Src/pre_processing_core.o ./Core/pre_processing_library/Src/pre_processing_core.su

.PHONY: clean-Core-2f-pre_processing_library-2f-Src

