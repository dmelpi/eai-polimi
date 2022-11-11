################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/leonardoiacussi/STM32CubeIDE/workspace_1.9.0/AI_STWIN_BM/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
/Users/leonardoiacussi/STM32CubeIDE/workspace_1.9.0/AI_STWIN_BM/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
/Users/leonardoiacussi/STM32CubeIDE/workspace_1.9.0/AI_STWIN_BM/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c 

OBJS += \
./Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.o \
./Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.o \
./Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.o 

C_DEPS += \
./Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.d \
./Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.d \
./Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.o: /Users/leonardoiacussi/STM32CubeIDE/workspace_1.9.0/AI_STWIN_BM/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32L4R9xx -c -I../../Core/pre_processing_library/Inc -I../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../Middlewares/ST/AI/Inc -I../../X-CUBE-AI/App -I../../Drivers/BSP/Components/Common -I../../Drivers/CMSIS/DSP/Include -I../../Drivers/BSP/STWIN -I../../Drivers/BSP/Components/ism330dhcx -I../../Core/Inc -I../../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../Drivers/CMSIS/Include -I../../USB_DEVICE/App -I../../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.o: /Users/leonardoiacussi/STM32CubeIDE/workspace_1.9.0/AI_STWIN_BM/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32L4R9xx -c -I../../Core/pre_processing_library/Inc -I../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../Middlewares/ST/AI/Inc -I../../X-CUBE-AI/App -I../../Drivers/BSP/Components/Common -I../../Drivers/CMSIS/DSP/Include -I../../Drivers/BSP/STWIN -I../../Drivers/BSP/Components/ism330dhcx -I../../Core/Inc -I../../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../Drivers/CMSIS/Include -I../../USB_DEVICE/App -I../../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.o: /Users/leonardoiacussi/STM32CubeIDE/workspace_1.9.0/AI_STWIN_BM/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c Middlewares/ST/STM32_USB_Device_Library/Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32L4R9xx -c -I../../Core/pre_processing_library/Inc -I../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../Middlewares/ST/AI/Inc -I../../X-CUBE-AI/App -I../../Drivers/BSP/Components/Common -I../../Drivers/CMSIS/DSP/Include -I../../Drivers/BSP/STWIN -I../../Drivers/BSP/Components/ism330dhcx -I../../Core/Inc -I../../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../Drivers/CMSIS/Include -I../../USB_DEVICE/App -I../../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-STM32_USB_Device_Library-2f-Core-2f-Src

clean-Middlewares-2f-ST-2f-STM32_USB_Device_Library-2f-Core-2f-Src:
	-$(RM) ./Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.d ./Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.o ./Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.su ./Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.d ./Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.o ./Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.su ./Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.d ./Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.o ./Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.su

.PHONY: clean-Middlewares-2f-ST-2f-STM32_USB_Device_Library-2f-Core-2f-Src

