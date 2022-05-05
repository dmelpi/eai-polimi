################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/leonardoiacussi/Desktop/PhD/ST_Motor/MFCC\ -\ JupyterNotebook\ -\ Firmware/Firmware_mfcc/Projects/STM32L4R9ZI-STWIN/Applications/FP-AI-MONITOR1/signal_processing_lib/lib_features_extraction/Src/ai_logging.c \
/Users/leonardoiacussi/Desktop/PhD/ST_Motor/MFCC\ -\ JupyterNotebook\ -\ Firmware/Firmware_mfcc/Projects/STM32L4R9ZI-STWIN/Applications/FP-AI-MONITOR1/signal_processing_lib/lib_features_extraction/Src/features_extraction.c 

OBJS += \
./Application/lib_features_extraction/Src/ai_logging.o \
./Application/lib_features_extraction/Src/features_extraction.o 

C_DEPS += \
./Application/lib_features_extraction/Src/ai_logging.d \
./Application/lib_features_extraction/Src/features_extraction.d 


# Each subdirectory must supply rules for building sources it contributes
Application/lib_features_extraction/Src/ai_logging.o: /Users/leonardoiacussi/Desktop/PhD/ST_Motor/MFCC\ -\ JupyterNotebook\ -\ Firmware/Firmware_mfcc/Projects/STM32L4R9ZI-STWIN/Applications/FP-AI-MONITOR1/signal_processing_lib/lib_features_extraction/Src/ai_logging.c Application/lib_features_extraction/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSYS_TP_MCU_STM32L4 -DSTM32L4R9xx -DSTM32L4R9ZIJx -DSTEVAL_STWINKT1 -DDEBUG -DSYS_DEBUG -DUSE_HAL_DRIVER -DUSE_FULL_ASSERT -DSYS_TRACE=0 -DSYS_DBG_ENABLE_TA4=0 -DARM_MATH_CM4 -D__FPU_PRESENT -c -I../../Inc -I../../X-CUBE-AI/App -I../../SensorManager/Inc -I../../signal_processing_lib/lib_features_extraction/Inc -I../../signal_processing_lib/gravity_filter/Inc -I../../DPU/Inc -I../../mx/Inc -I../../../../../../Drivers/CMSIS/Core/Include -I../../../../../../Drivers/CMSIS/DSP/Include -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/BSP/Components/iis3dwb -I../../../../../../Drivers/BSP/Components/ism330dhcx -I../../../../../../Drivers/BSP/Components/hts221 -I../../../../../../Drivers/BSP/Components/lps22hh -I../../../../../../Middlewares/ST/eLooM/Inc -I../../../../../../Middlewares/ST/NanoEdge_AI_Library/Inc -I../../../../../../Middlewares/ST/STM32_AI_Library/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../Middlewares/ST/STM32_GenericFFT_Library/Inc -I../../../../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I/FP-AI-MONITOR1_prj/Application/feature_extraction_library/Inc -I"/Users/leonardoiacussi/Desktop/PhD/ST_Motor/MFCC - JupyterNotebook - Firmware/Firmware_mfcc/Projects/STM32L4R9ZI-STWIN/Applications/FP-AI-MONITOR1/signal_processing_lib/feature_extraction_library/Inc" -O0 -ffunction-sections -fdata-sections -Wall -include ../../Inc/sysconfig.h -fstack-usage -MMD -MP -MF"Application/lib_features_extraction/Src/ai_logging.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Application/lib_features_extraction/Src/features_extraction.o: /Users/leonardoiacussi/Desktop/PhD/ST_Motor/MFCC\ -\ JupyterNotebook\ -\ Firmware/Firmware_mfcc/Projects/STM32L4R9ZI-STWIN/Applications/FP-AI-MONITOR1/signal_processing_lib/lib_features_extraction/Src/features_extraction.c Application/lib_features_extraction/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSYS_TP_MCU_STM32L4 -DSTM32L4R9xx -DSTM32L4R9ZIJx -DSTEVAL_STWINKT1 -DDEBUG -DSYS_DEBUG -DUSE_HAL_DRIVER -DUSE_FULL_ASSERT -DSYS_TRACE=0 -DSYS_DBG_ENABLE_TA4=0 -DARM_MATH_CM4 -D__FPU_PRESENT -c -I../../Inc -I../../X-CUBE-AI/App -I../../SensorManager/Inc -I../../signal_processing_lib/lib_features_extraction/Inc -I../../signal_processing_lib/gravity_filter/Inc -I../../DPU/Inc -I../../mx/Inc -I../../../../../../Drivers/CMSIS/Core/Include -I../../../../../../Drivers/CMSIS/DSP/Include -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/BSP/Components/iis3dwb -I../../../../../../Drivers/BSP/Components/ism330dhcx -I../../../../../../Drivers/BSP/Components/hts221 -I../../../../../../Drivers/BSP/Components/lps22hh -I../../../../../../Middlewares/ST/eLooM/Inc -I../../../../../../Middlewares/ST/NanoEdge_AI_Library/Inc -I../../../../../../Middlewares/ST/STM32_AI_Library/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../Middlewares/ST/STM32_GenericFFT_Library/Inc -I../../../../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I/FP-AI-MONITOR1_prj/Application/feature_extraction_library/Inc -I"/Users/leonardoiacussi/Desktop/PhD/ST_Motor/MFCC - JupyterNotebook - Firmware/Firmware_mfcc/Projects/STM32L4R9ZI-STWIN/Applications/FP-AI-MONITOR1/signal_processing_lib/feature_extraction_library/Inc" -O0 -ffunction-sections -fdata-sections -Wall -include ../../Inc/sysconfig.h -fstack-usage -MMD -MP -MF"Application/lib_features_extraction/Src/features_extraction.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-lib_features_extraction-2f-Src

clean-Application-2f-lib_features_extraction-2f-Src:
	-$(RM) ./Application/lib_features_extraction/Src/ai_logging.d ./Application/lib_features_extraction/Src/ai_logging.o ./Application/lib_features_extraction/Src/ai_logging.su ./Application/lib_features_extraction/Src/features_extraction.d ./Application/lib_features_extraction/Src/features_extraction.o ./Application/lib_features_extraction/Src/features_extraction.su

.PHONY: clean-Application-2f-lib_features_extraction-2f-Src

