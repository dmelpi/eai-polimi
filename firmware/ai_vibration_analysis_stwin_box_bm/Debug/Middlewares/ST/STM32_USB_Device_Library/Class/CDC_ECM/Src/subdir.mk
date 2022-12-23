################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STM32_USB_Device_Library/Class/CDC_ECM/Src/usbd_cdc_ecm.c \
../Middlewares/ST/STM32_USB_Device_Library/Class/CDC_ECM/Src/usbd_cdc_ecm_if_template.c 

OBJS += \
./Middlewares/ST/STM32_USB_Device_Library/Class/CDC_ECM/Src/usbd_cdc_ecm.o \
./Middlewares/ST/STM32_USB_Device_Library/Class/CDC_ECM/Src/usbd_cdc_ecm_if_template.o 

C_DEPS += \
./Middlewares/ST/STM32_USB_Device_Library/Class/CDC_ECM/Src/usbd_cdc_ecm.d \
./Middlewares/ST/STM32_USB_Device_Library/Class/CDC_ECM/Src/usbd_cdc_ecm_if_template.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_USB_Device_Library/Class/CDC_ECM/Src/%.o Middlewares/ST/STM32_USB_Device_Library/Class/CDC_ECM/Src/%.su: ../Middlewares/ST/STM32_USB_Device_Library/Class/CDC_ECM/Src/%.c Middlewares/ST/STM32_USB_Device_Library/Class/CDC_ECM/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32U585xx -c -I../Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Core/pre_processing_library/Inc -I../Drivers/BSP/STWIN.box -I../Drivers/BSP/Components/ism330dhcx -I../Drivers/BSP/Components/Common -I../Drivers/STM32U5xx_HAL_Driver/Inc -I../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../Drivers/CMSIS/Include -I../Drivers/CMSIS/DSP/Include -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC -I../USB_Device/Target -I../USB_Device/App -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-ST-2f-STM32_USB_Device_Library-2f-Class-2f-CDC_ECM-2f-Src

clean-Middlewares-2f-ST-2f-STM32_USB_Device_Library-2f-Class-2f-CDC_ECM-2f-Src:
	-$(RM) ./Middlewares/ST/STM32_USB_Device_Library/Class/CDC_ECM/Src/usbd_cdc_ecm.d ./Middlewares/ST/STM32_USB_Device_Library/Class/CDC_ECM/Src/usbd_cdc_ecm.o ./Middlewares/ST/STM32_USB_Device_Library/Class/CDC_ECM/Src/usbd_cdc_ecm.su ./Middlewares/ST/STM32_USB_Device_Library/Class/CDC_ECM/Src/usbd_cdc_ecm_if_template.d ./Middlewares/ST/STM32_USB_Device_Library/Class/CDC_ECM/Src/usbd_cdc_ecm_if_template.o ./Middlewares/ST/STM32_USB_Device_Library/Class/CDC_ECM/Src/usbd_cdc_ecm_if_template.su

.PHONY: clean-Middlewares-2f-ST-2f-STM32_USB_Device_Library-2f-Class-2f-CDC_ECM-2f-Src

