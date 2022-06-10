################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include eLooM_Components/SensorManager/Src/services/subdir.mk
-include eLooM_Components/SensorManager/Src/events/subdir.mk
-include eLooM_Components/SensorManager/Src/drivers/subdir.mk
-include eLooM_Components/SensorManager/Src/subdir.mk
-include eLooM_Components/DPU/Src/events/subdir.mk
-include eLooM_Components/DPU/Src/subdir.mk
-include User/mx/Src/subdir.mk
-include User/CubeAI/Src/subdir.mk
-include User/Core/Startup/subdir.mk
-include User/Core/Src/services/ai_data_format/subdir.mk
-include User/Core/Src/drivers/subdir.mk
-include User/Core/Src/subdir.mk
-include Middlewares/ST/threadx/ports/cortex_m33/gnu/src/subdir.mk
-include Middlewares/ST/threadx/common/src/subdir.mk
-include Middlewares/ST/eLooM/Src/services/subdir.mk
-include Middlewares/ST/eLooM/Src/events/subdir.mk
-include Middlewares/ST/STM32_GenericFFT_Library/subdir.mk
-include Drivers/STM32U5xx_HAL_Driver/Src/subdir.mk
-include Drivers/Components/ism330dhcx/subdir.mk
-include Drivers/Components/iis3dwb/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := AiDpuDemo_stwin_box_prj
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
AiDpuDemo_stwin_box_prj.elf \

MAP_FILES += \
AiDpuDemo_stwin_box_prj.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
AiDpuDemo_stwin_box_prj.list \


# All Target
all: main-build

# Main-build Target
main-build: AiDpuDemo_stwin_box_prj.elf secondary-outputs

# Tool invocations
AiDpuDemo_stwin_box_prj.elf AiDpuDemo_stwin_box_prj.map: $(OBJS) $(USER_OBJS) /Users/leonardoiacussi/eai-polimi/ai_dpu_stwin_box/Projects/STWIN.box/Applications/AiDpuDemo_prj/STM32CubeIDE/STM32U585AIIXQ_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "AiDpuDemo_stwin_box_prj.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m33 -T"/Users/leonardoiacussi/eai-polimi/ai_dpu_stwin_box/Projects/STWIN.box/Applications/AiDpuDemo_prj/STM32CubeIDE/STM32U585AIIXQ_FLASH.ld" --specs=nosys.specs -Wl,-Map="AiDpuDemo_stwin_box_prj.map" -Wl,--gc-sections -static -L../../../../../../Drivers/CMSIS/DSP/Lib/GCC -L../../X-CUBE-AI/Lib --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

AiDpuDemo_stwin_box_prj.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "AiDpuDemo_stwin_box_prj.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) AiDpuDemo_stwin_box_prj.elf AiDpuDemo_stwin_box_prj.list AiDpuDemo_stwin_box_prj.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
