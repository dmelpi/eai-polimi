################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/leonardoiacussi/Desktop/PhD/Firmware/Projects/STM32L4R9ZI-STWIN/Applications/FP-AI-MONITOR1/SensorManager/Src/events/ISensorEventListener.c \
/Users/leonardoiacussi/Desktop/PhD/Firmware/Projects/STM32L4R9ZI-STWIN/Applications/FP-AI-MONITOR1/SensorManager/Src/events/SensorEvent.c \
/Users/leonardoiacussi/Desktop/PhD/Firmware/Projects/STM32L4R9ZI-STWIN/Applications/FP-AI-MONITOR1/SensorManager/Src/events/SensorEventSrc.c 

OBJS += \
./Application/SensorManager/Src/events/ISensorEventListener.o \
./Application/SensorManager/Src/events/SensorEvent.o \
./Application/SensorManager/Src/events/SensorEventSrc.o 

C_DEPS += \
./Application/SensorManager/Src/events/ISensorEventListener.d \
./Application/SensorManager/Src/events/SensorEvent.d \
./Application/SensorManager/Src/events/SensorEventSrc.d 


# Each subdirectory must supply rules for building sources it contributes
Application/SensorManager/Src/events/ISensorEventListener.o: /Users/leonardoiacussi/Desktop/PhD/Firmware/Projects/STM32L4R9ZI-STWIN/Applications/FP-AI-MONITOR1/SensorManager/Src/events/ISensorEventListener.c Application/SensorManager/Src/events/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSYS_TP_MCU_STM32L4 -DSTM32L4R9xx -DSTM32L4R9ZIJx -DSTEVAL_STWINKT1 -DDEBUG -DSYS_DEBUG -DUSE_HAL_DRIVER -DUSE_FULL_ASSERT -DSYS_TRACE=0 -DSYS_DBG_ENABLE_TA4=0 -DARM_MATH_CM4 -D__FPU_PRESENT -c -I../../Inc -I../../X-CUBE-AI/App -I../../SensorManager/Inc -I../../signal_processing_lib/lib_features_extraction/Inc -I../../signal_processing_lib/gravity_filter/Inc -I../../DPU/Inc -I../../mx/Inc -I../../../../../../Drivers/CMSIS/Core/Include -I../../../../../../Drivers/CMSIS/DSP/Include -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/BSP/Components/iis3dwb -I../../../../../../Drivers/BSP/Components/ism330dhcx -I../../../../../../Drivers/BSP/Components/hts221 -I../../../../../../Drivers/BSP/Components/lps22hh -I../../../../../../Middlewares/ST/eLooM/Inc -I../../../../../../Middlewares/ST/NanoEdge_AI_Library/Inc -I../../../../../../Middlewares/ST/STM32_AI_Library/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../Middlewares/ST/STM32_GenericFFT_Library/Inc -I../../../../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -include ../../Inc/sysconfig.h -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/SensorManager/Src/events/SensorEvent.o: /Users/leonardoiacussi/Desktop/PhD/Firmware/Projects/STM32L4R9ZI-STWIN/Applications/FP-AI-MONITOR1/SensorManager/Src/events/SensorEvent.c Application/SensorManager/Src/events/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSYS_TP_MCU_STM32L4 -DSTM32L4R9xx -DSTM32L4R9ZIJx -DSTEVAL_STWINKT1 -DDEBUG -DSYS_DEBUG -DUSE_HAL_DRIVER -DUSE_FULL_ASSERT -DSYS_TRACE=0 -DSYS_DBG_ENABLE_TA4=0 -DARM_MATH_CM4 -D__FPU_PRESENT -c -I../../Inc -I../../X-CUBE-AI/App -I../../SensorManager/Inc -I../../signal_processing_lib/lib_features_extraction/Inc -I../../signal_processing_lib/gravity_filter/Inc -I../../DPU/Inc -I../../mx/Inc -I../../../../../../Drivers/CMSIS/Core/Include -I../../../../../../Drivers/CMSIS/DSP/Include -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/BSP/Components/iis3dwb -I../../../../../../Drivers/BSP/Components/ism330dhcx -I../../../../../../Drivers/BSP/Components/hts221 -I../../../../../../Drivers/BSP/Components/lps22hh -I../../../../../../Middlewares/ST/eLooM/Inc -I../../../../../../Middlewares/ST/NanoEdge_AI_Library/Inc -I../../../../../../Middlewares/ST/STM32_AI_Library/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../Middlewares/ST/STM32_GenericFFT_Library/Inc -I../../../../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -include ../../Inc/sysconfig.h -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/SensorManager/Src/events/SensorEventSrc.o: /Users/leonardoiacussi/Desktop/PhD/Firmware/Projects/STM32L4R9ZI-STWIN/Applications/FP-AI-MONITOR1/SensorManager/Src/events/SensorEventSrc.c Application/SensorManager/Src/events/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSYS_TP_MCU_STM32L4 -DSTM32L4R9xx -DSTM32L4R9ZIJx -DSTEVAL_STWINKT1 -DDEBUG -DSYS_DEBUG -DUSE_HAL_DRIVER -DUSE_FULL_ASSERT -DSYS_TRACE=0 -DSYS_DBG_ENABLE_TA4=0 -DARM_MATH_CM4 -D__FPU_PRESENT -c -I../../Inc -I../../X-CUBE-AI/App -I../../SensorManager/Inc -I../../signal_processing_lib/lib_features_extraction/Inc -I../../signal_processing_lib/gravity_filter/Inc -I../../DPU/Inc -I../../mx/Inc -I../../../../../../Drivers/CMSIS/Core/Include -I../../../../../../Drivers/CMSIS/DSP/Include -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/BSP/Components/iis3dwb -I../../../../../../Drivers/BSP/Components/ism330dhcx -I../../../../../../Drivers/BSP/Components/hts221 -I../../../../../../Drivers/BSP/Components/lps22hh -I../../../../../../Middlewares/ST/eLooM/Inc -I../../../../../../Middlewares/ST/NanoEdge_AI_Library/Inc -I../../../../../../Middlewares/ST/STM32_AI_Library/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../Middlewares/ST/STM32_GenericFFT_Library/Inc -I../../../../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -include ../../Inc/sysconfig.h -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-SensorManager-2f-Src-2f-events

clean-Application-2f-SensorManager-2f-Src-2f-events:
	-$(RM) ./Application/SensorManager/Src/events/ISensorEventListener.d ./Application/SensorManager/Src/events/ISensorEventListener.o ./Application/SensorManager/Src/events/ISensorEventListener.su ./Application/SensorManager/Src/events/SensorEvent.d ./Application/SensorManager/Src/events/SensorEvent.o ./Application/SensorManager/Src/events/SensorEvent.su ./Application/SensorManager/Src/events/SensorEventSrc.d ./Application/SensorManager/Src/events/SensorEventSrc.o ./Application/SensorManager/Src/events/SensorEventSrc.su

.PHONY: clean-Application-2f-SensorManager-2f-Src-2f-events

