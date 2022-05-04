################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Application/Startup/startup_stm32l4r9zijx.s 

C_SRCS += \
../Application/Startup/syscalls.c \
../Application/Startup/sysmem.c 

OBJS += \
./Application/Startup/startup_stm32l4r9zijx.o \
./Application/Startup/syscalls.o \
./Application/Startup/sysmem.o 

S_DEPS += \
./Application/Startup/startup_stm32l4r9zijx.d 

C_DEPS += \
./Application/Startup/syscalls.d \
./Application/Startup/sysmem.d 


# Each subdirectory must supply rules for building sources it contributes
Application/Startup/%.o: ../Application/Startup/%.s Application/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DSYS_TP_MCU_STM32L4 -DSTM32L4R9xx -DUSE_HAL_DRIVER -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Application/Startup/%.o Application/Startup/%.su: ../Application/Startup/%.c Application/Startup/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSYS_TP_MCU_STM32L4 -DSTM32L4R9xx -DSTM32L4R9ZIJx -DSTEVAL_STWINKT1 -DDEBUG -DSYS_DEBUG -DUSE_HAL_DRIVER -DUSE_FULL_ASSERT -DSYS_TRACE=0 -DSYS_DBG_ENABLE_TA4=0 -DARM_MATH_CM4 -D__FPU_PRESENT -c -I../../Inc -I../../X-CUBE-AI/App -I../../SensorManager/Inc -I../../signal_processing_lib/lib_features_extraction/Inc -I../../signal_processing_lib/gravity_filter/Inc -I../../DPU/Inc -I../../mx/Inc -I../../../../../../Drivers/CMSIS/Core/Include -I../../../../../../Drivers/CMSIS/DSP/Include -I../../../../../../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../../../../../../Drivers/STM32L4xx_HAL_Driver/Inc -I../../../../../../Drivers/BSP/Components/iis3dwb -I../../../../../../Drivers/BSP/Components/ism330dhcx -I../../../../../../Drivers/BSP/Components/hts221 -I../../../../../../Drivers/BSP/Components/lps22hh -I../../../../../../Middlewares/ST/eLooM/Inc -I../../../../../../Middlewares/ST/NanoEdge_AI_Library/Inc -I../../../../../../Middlewares/ST/STM32_AI_Library/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../../../../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../../../../../../Middlewares/ST/STM32_GenericFFT_Library/Inc -I../../../../../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../../../../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -include ../../Inc/sysconfig.h -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-Startup

clean-Application-2f-Startup:
	-$(RM) ./Application/Startup/startup_stm32l4r9zijx.d ./Application/Startup/startup_stm32l4r9zijx.o ./Application/Startup/syscalls.d ./Application/Startup/syscalls.o ./Application/Startup/syscalls.su ./Application/Startup/sysmem.d ./Application/Startup/sysmem.o ./Application/Startup/sysmem.su

.PHONY: clean-Application-2f-Startup

