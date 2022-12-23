################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/leonardoiacussi/STM32CubeIDE/workspace_1.9.0/AI_STWIN_BM/Core/pre_processing_library/Src/pre_processing_app.c \
/Users/leonardoiacussi/STM32CubeIDE/workspace_1.9.0/AI_STWIN_BM/Core/pre_processing_library/Src/pre_processing_core.c 

OBJS += \
./Core/pre_processing_library/Src/pre_processing_app.o \
./Core/pre_processing_library/Src/pre_processing_core.o 

C_DEPS += \
./Core/pre_processing_library/Src/pre_processing_app.d \
./Core/pre_processing_library/Src/pre_processing_core.d 


# Each subdirectory must supply rules for building sources it contributes
Core/pre_processing_library/Src/pre_processing_app.o: /Users/leonardoiacussi/STM32CubeIDE/workspace_1.9.0/AI_STWIN_BM/Core/pre_processing_library/Src/pre_processing_app.c Core/pre_processing_library/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32L4R9xx -c -I../../Core/pre_processing_library/Inc -I../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../Middlewares/ST/AI/Inc -I../../X-CUBE-AI/App -I../../Drivers/BSP/Components/Common -I../../Drivers/CMSIS/DSP/Include -I../../Drivers/BSP/STWIN -I../../Drivers/BSP/Components/ism330dhcx -I../../Core/Inc -I../../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../Drivers/CMSIS/Include -I../../USB_DEVICE/App -I../../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/pre_processing_library/Src/pre_processing_core.o: /Users/leonardoiacussi/STM32CubeIDE/workspace_1.9.0/AI_STWIN_BM/Core/pre_processing_library/Src/pre_processing_core.c Core/pre_processing_library/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32L4R9xx -c -I../../Core/pre_processing_library/Inc -I../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../Middlewares/ST/AI/Inc -I../../X-CUBE-AI/App -I../../Drivers/BSP/Components/Common -I../../Drivers/CMSIS/DSP/Include -I../../Drivers/BSP/STWIN -I../../Drivers/BSP/Components/ism330dhcx -I../../Core/Inc -I../../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../Drivers/CMSIS/Include -I../../USB_DEVICE/App -I../../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-pre_processing_library-2f-Src

clean-Core-2f-pre_processing_library-2f-Src:
	-$(RM) ./Core/pre_processing_library/Src/pre_processing_app.d ./Core/pre_processing_library/Src/pre_processing_app.o ./Core/pre_processing_library/Src/pre_processing_app.su ./Core/pre_processing_library/Src/pre_processing_core.d ./Core/pre_processing_library/Src/pre_processing_core.o ./Core/pre_processing_library/Src/pre_processing_core.su

.PHONY: clean-Core-2f-pre_processing_library-2f-Src

