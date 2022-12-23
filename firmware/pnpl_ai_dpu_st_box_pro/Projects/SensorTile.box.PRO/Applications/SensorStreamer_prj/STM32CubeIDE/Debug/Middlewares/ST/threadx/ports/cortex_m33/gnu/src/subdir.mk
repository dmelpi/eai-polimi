################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
/Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_restore.S \
/Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_save.S \
/Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_control.S \
/Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_schedule.S \
/Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_build.S \
/Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_system_return.S \
/Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_timer_interrupt.S 

OBJS += \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_restore.o \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_save.o \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_control.o \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_schedule.o \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_build.o \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_system_return.o \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_timer_interrupt.o 

S_UPPER_DEPS += \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_restore.d \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_save.d \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_control.d \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_schedule.d \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_build.d \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_system_return.d \
./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_timer_interrupt.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_restore.o: /Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_restore.S Middlewares/ST/threadx/ports/cortex_m33/gnu/src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m33 -g3 -DDEBUG -DUSE_DYNAMIC_MEMORY_ALLOCATION -DTX_SINGLE_MODE_NON_SECURE=1 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_save.o: /Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_save.S Middlewares/ST/threadx/ports/cortex_m33/gnu/src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m33 -g3 -DDEBUG -DUSE_DYNAMIC_MEMORY_ALLOCATION -DTX_SINGLE_MODE_NON_SECURE=1 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_control.o: /Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_control.S Middlewares/ST/threadx/ports/cortex_m33/gnu/src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m33 -g3 -DDEBUG -DUSE_DYNAMIC_MEMORY_ALLOCATION -DTX_SINGLE_MODE_NON_SECURE=1 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_schedule.o: /Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_schedule.S Middlewares/ST/threadx/ports/cortex_m33/gnu/src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m33 -g3 -DDEBUG -DUSE_DYNAMIC_MEMORY_ALLOCATION -DTX_SINGLE_MODE_NON_SECURE=1 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_build.o: /Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_build.S Middlewares/ST/threadx/ports/cortex_m33/gnu/src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m33 -g3 -DDEBUG -DUSE_DYNAMIC_MEMORY_ALLOCATION -DTX_SINGLE_MODE_NON_SECURE=1 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_system_return.o: /Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_system_return.S Middlewares/ST/threadx/ports/cortex_m33/gnu/src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m33 -g3 -DDEBUG -DUSE_DYNAMIC_MEMORY_ALLOCATION -DTX_SINGLE_MODE_NON_SECURE=1 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"
Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_timer_interrupt.o: /Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_timer_interrupt.S Middlewares/ST/threadx/ports/cortex_m33/gnu/src/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m33 -g3 -DDEBUG -DUSE_DYNAMIC_MEMORY_ALLOCATION -DTX_SINGLE_MODE_NON_SECURE=1 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-Middlewares-2f-ST-2f-threadx-2f-ports-2f-cortex_m33-2f-gnu-2f-src

clean-Middlewares-2f-ST-2f-threadx-2f-ports-2f-cortex_m33-2f-gnu-2f-src:
	-$(RM) ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_restore.d ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_restore.o ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_save.d ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_context_save.o ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_control.d ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_interrupt_control.o ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_schedule.d ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_schedule.o ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_build.d ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_stack_build.o ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_system_return.d ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_thread_system_return.o ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_timer_interrupt.d ./Middlewares/ST/threadx/ports/cortex_m33/gnu/src/tx_timer_interrupt.o

.PHONY: clean-Middlewares-2f-ST-2f-threadx-2f-ports-2f-cortex_m33-2f-gnu-2f-src

