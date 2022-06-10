################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/leonardoiacussi/eai-polimi/ai_dpu_stwin_box/Projects/eLooM_Components/SensorManager/Src/drivers/SMUtilityDriver.c \
/Users/leonardoiacussi/eai-polimi/ai_dpu_stwin_box/Projects/eLooM_Components/SensorManager/Src/drivers/SPIMasterDriver.c 

OBJS += \
./eLooM_Components/SensorManager/Src/drivers/SMUtilityDriver.o \
./eLooM_Components/SensorManager/Src/drivers/SPIMasterDriver.o 

C_DEPS += \
./eLooM_Components/SensorManager/Src/drivers/SMUtilityDriver.d \
./eLooM_Components/SensorManager/Src/drivers/SPIMasterDriver.d 


# Each subdirectory must supply rules for building sources it contributes
eLooM_Components/SensorManager/Src/drivers/SMUtilityDriver.o: /Users/leonardoiacussi/eai-polimi/ai_dpu_stwin_box/Projects/eLooM_Components/SensorManager/Src/drivers/SMUtilityDriver.c eLooM_Components/SensorManager/Src/drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM33 -DSYS_DEBUG -DUSE_DYNAMIC_MEMORY_ALLOCATION -DSYS_TP_MCU_STM32U5 -DDEBUG -DUSE_FULL_ASSERT -DSYS_TRACE=0 -DSYS_DBG_ENABLE_TA4=0 -DTX_INCLUDE_USER_DEFINE_FILE -DTX_SINGLE_MODE_NON_SECURE=1 -DUSE_HAL_DRIVER -DSTM32U585xx -c -I../../Core/Inc -I../../X-CUBE-AI/App -I../../X-CUBE-AI -I../../Core/Inc/services/ai_data_format -I../../mx/Inc -I../../../../../../Projects/eLooM_Components/DPU/Inc -I../../../../../../Projects/eLooM_Components/SensorManager/Inc -I../../../../../../Middlewares/ST/threadx/common/inc -I../../../../../../Middlewares/ST/eLooM/Inc -I../../../../../../Middlewares/ST/STM32_AI_Library/Inc -I../../../../../../Middlewares/ST/STM32_GenericFFT_Library/Inc -I../../../../../../Middlewares/ST/threadx/ports/cortex_m33/gnu/inc -I../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc -I../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../../../../../../Drivers/CMSIS/DSP/Include -I../../../../../../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/iis3dwb -I../../../../../../Drivers/BSP/Components/ism330dhcx -O0 -ffunction-sections -fdata-sections -Wall -include ../../Core/Inc/sysconfig.h -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
eLooM_Components/SensorManager/Src/drivers/SPIMasterDriver.o: /Users/leonardoiacussi/eai-polimi/ai_dpu_stwin_box/Projects/eLooM_Components/SensorManager/Src/drivers/SPIMasterDriver.c eLooM_Components/SensorManager/Src/drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM33 -DSYS_DEBUG -DUSE_DYNAMIC_MEMORY_ALLOCATION -DSYS_TP_MCU_STM32U5 -DDEBUG -DUSE_FULL_ASSERT -DSYS_TRACE=0 -DSYS_DBG_ENABLE_TA4=0 -DTX_INCLUDE_USER_DEFINE_FILE -DTX_SINGLE_MODE_NON_SECURE=1 -DUSE_HAL_DRIVER -DSTM32U585xx -c -I../../Core/Inc -I../../X-CUBE-AI/App -I../../X-CUBE-AI -I../../Core/Inc/services/ai_data_format -I../../mx/Inc -I../../../../../../Projects/eLooM_Components/DPU/Inc -I../../../../../../Projects/eLooM_Components/SensorManager/Inc -I../../../../../../Middlewares/ST/threadx/common/inc -I../../../../../../Middlewares/ST/eLooM/Inc -I../../../../../../Middlewares/ST/STM32_AI_Library/Inc -I../../../../../../Middlewares/ST/STM32_GenericFFT_Library/Inc -I../../../../../../Middlewares/ST/threadx/ports/cortex_m33/gnu/inc -I../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc -I../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../../../../../../Drivers/CMSIS/DSP/Include -I../../../../../../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/BSP/Components/iis3dwb -I../../../../../../Drivers/BSP/Components/ism330dhcx -O0 -ffunction-sections -fdata-sections -Wall -include ../../Core/Inc/sysconfig.h -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-eLooM_Components-2f-SensorManager-2f-Src-2f-drivers

clean-eLooM_Components-2f-SensorManager-2f-Src-2f-drivers:
	-$(RM) ./eLooM_Components/SensorManager/Src/drivers/SMUtilityDriver.d ./eLooM_Components/SensorManager/Src/drivers/SMUtilityDriver.o ./eLooM_Components/SensorManager/Src/drivers/SMUtilityDriver.su ./eLooM_Components/SensorManager/Src/drivers/SPIMasterDriver.d ./eLooM_Components/SensorManager/Src/drivers/SPIMasterDriver.o ./eLooM_Components/SensorManager/Src/drivers/SPIMasterDriver.su

.PHONY: clean-eLooM_Components-2f-SensorManager-2f-Src-2f-drivers

