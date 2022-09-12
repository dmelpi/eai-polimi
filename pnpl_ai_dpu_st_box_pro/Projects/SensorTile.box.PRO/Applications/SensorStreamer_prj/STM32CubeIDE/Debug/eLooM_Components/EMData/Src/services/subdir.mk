################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Projects/eLooM_Components/EMData/Src/services/CircularBuffer.c \
/Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Projects/eLooM_Components/EMData/Src/services/em_data_format.c 

OBJS += \
./eLooM_Components/EMData/Src/services/CircularBuffer.o \
./eLooM_Components/EMData/Src/services/em_data_format.o 

C_DEPS += \
./eLooM_Components/EMData/Src/services/CircularBuffer.d \
./eLooM_Components/EMData/Src/services/em_data_format.d 


# Each subdirectory must supply rules for building sources it contributes
eLooM_Components/EMData/Src/services/CircularBuffer.o: /Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Projects/eLooM_Components/EMData/Src/services/CircularBuffer.c eLooM_Components/EMData/Src/services/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DSYS_DEBUG -DUSE_DYNAMIC_MEMORY_ALLOCATION -DSYS_TP_MCU_STM32U5 -DDEBUG -DUSE_FULL_ASSERT -DSYS_TRACE=0 -DSYS_DBG_ENABLE_TA4=0 -DTX_INCLUDE_USER_DEFINE_FILE -DTX_SINGLE_MODE_NON_SECURE=1 -DUSE_HAL_DRIVER -DSTM32U585xx -DARM_MATH_CM33 -c -I../../Inc -I../../mx/Inc -I../../../../../../Middlewares/ST/threadx/common/inc -I../../../../../../Middlewares/ST/eLooM/Inc -I../../../../../../Middlewares/ST/threadx/ports/cortex_m33/gnu/inc -I../../../../../../Drivers/BSP/Components/hts221 -I../../../../../../Drivers/BSP/Components/stts22h -I../../../../../../Drivers/BSP/Components/lis2mdl -I../../../../../../Drivers/BSP/Components/lps22df -I../../../../../../Drivers/BSP/Components/lis2du12 -I../../../../../../Drivers/BSP/Components/lsm6dsv16x -I../../../../../../Drivers/BSP/Components/iis2iclx -I../../../../../../Drivers/BSP/Components/ism330dhcx -I../../../../../../Drivers/BSP/Components/iis3dwb -I../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc -I../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../../../../../../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/CMSIS/DSP/Include -I../../../../../../Projects/eLooM_Components/SensorManager/Inc -I../../../../../../Projects/eLooM_Components/EMData/Inc -include../../Inc/sysconfig.h -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
eLooM_Components/EMData/Src/services/em_data_format.o: /Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Projects/eLooM_Components/EMData/Src/services/em_data_format.c eLooM_Components/EMData/Src/services/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DSYS_DEBUG -DUSE_DYNAMIC_MEMORY_ALLOCATION -DSYS_TP_MCU_STM32U5 -DDEBUG -DUSE_FULL_ASSERT -DSYS_TRACE=0 -DSYS_DBG_ENABLE_TA4=0 -DTX_INCLUDE_USER_DEFINE_FILE -DTX_SINGLE_MODE_NON_SECURE=1 -DUSE_HAL_DRIVER -DSTM32U585xx -DARM_MATH_CM33 -c -I../../Inc -I../../mx/Inc -I../../../../../../Middlewares/ST/threadx/common/inc -I../../../../../../Middlewares/ST/eLooM/Inc -I../../../../../../Middlewares/ST/threadx/ports/cortex_m33/gnu/inc -I../../../../../../Drivers/BSP/Components/hts221 -I../../../../../../Drivers/BSP/Components/stts22h -I../../../../../../Drivers/BSP/Components/lis2mdl -I../../../../../../Drivers/BSP/Components/lps22df -I../../../../../../Drivers/BSP/Components/lis2du12 -I../../../../../../Drivers/BSP/Components/lsm6dsv16x -I../../../../../../Drivers/BSP/Components/iis2iclx -I../../../../../../Drivers/BSP/Components/ism330dhcx -I../../../../../../Drivers/BSP/Components/iis3dwb -I../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc -I../../../../../../Drivers/STM32U5xx_HAL_Driver/Inc/Legacy -I../../../../../../Drivers/CMSIS/Device/ST/STM32U5xx/Include -I../../../../../../Drivers/CMSIS/Include -I../../../../../../Drivers/CMSIS/DSP/Include -I../../../../../../Projects/eLooM_Components/SensorManager/Inc -I../../../../../../Projects/eLooM_Components/EMData/Inc -include../../Inc/sysconfig.h -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-eLooM_Components-2f-EMData-2f-Src-2f-services

clean-eLooM_Components-2f-EMData-2f-Src-2f-services:
	-$(RM) ./eLooM_Components/EMData/Src/services/CircularBuffer.d ./eLooM_Components/EMData/Src/services/CircularBuffer.o ./eLooM_Components/EMData/Src/services/CircularBuffer.su ./eLooM_Components/EMData/Src/services/em_data_format.d ./eLooM_Components/EMData/Src/services/em_data_format.o ./eLooM_Components/EMData/Src/services/em_data_format.su

.PHONY: clean-eLooM_Components-2f-EMData-2f-Src-2f-services

