################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include eLooM_Components/SensorManager/Src/services/subdir.mk
-include eLooM_Components/SensorManager/Src/drivers/subdir.mk
-include eLooM_Components/SensorManager/Src/subdir.mk
-include eLooM_Components/EMData/Src/services/subdir.mk
-include eLooM_Components/EMData/Src/events/subdir.mk
-include Middlewares/ST/threadx/ports/cortex_m33/gnu/src/subdir.mk
-include Middlewares/ST/threadx/common/src/subdir.mk
-include Middlewares/ST/eLooM/Src/services/subdir.mk
-include Middlewares/ST/eLooM/Src/events/subdir.mk
-include Middlewares/ST/eLooM/Src/drivers/subdir.mk
-include Drivers/STM32U5xx_HAL_Driver/subdir.mk
-include Drivers/BSP/Components/stts22h/subdir.mk
-include Drivers/BSP/Components/lsm6dsv16x/subdir.mk
-include Drivers/BSP/Components/lps22df/subdir.mk
-include Drivers/BSP/Components/lis2mdl/subdir.mk
-include Drivers/BSP/Components/lis2du12/subdir.mk
-include Drivers/BSP/Components/ism330dhcx/subdir.mk
-include Drivers/BSP/Components/ilps22qs/subdir.mk
-include Drivers/BSP/Components/iis3dwb/subdir.mk
-include Drivers/BSP/Components/iis2mdc/subdir.mk
-include Drivers/BSP/Components/iis2iclx/subdir.mk
-include Drivers/BSP/Components/iis2dlpc/subdir.mk
-include Drivers/BSP/Components/hts221/subdir.mk
-include Application/mx/Src/subdir.mk
-include Application/Startup/subdir.mk
-include Application/Src/events/subdir.mk
-include Application/Src/drivers/subdir.mk
-include Application/Src/subdir.mk
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


BUILD_ARTIFACT_NAME := SensorTile_box_PRO_SensorStreamer_prj
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
SensorTile_box_PRO_SensorStreamer_prj.elf \

MAP_FILES += \
SensorTile_box_PRO_SensorStreamer_prj.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
SensorTile_box_PRO_SensorStreamer_prj.list \

OBJCOPY_BIN += \
SensorTile_box_PRO_SensorStreamer_prj.bin \


# All Target
all: main-build

# Main-build Target
main-build: SensorTile_box_PRO_SensorStreamer_prj.elf secondary-outputs

# Tool invocations
SensorTile_box_PRO_SensorStreamer_prj.elf SensorTile_box_PRO_SensorStreamer_prj.map: $(OBJS) $(USER_OBJS) /Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Projects/SensorTile.box.PRO/Applications/SensorStreamer_prj/STM32CubeIDE/STM32U585AIIXQ_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "SensorTile_box_PRO_SensorStreamer_prj.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m33 -T"/Users/avilla/devel/projects/eai-polimi/pnpl_ai_dpu_st_box_pro/Projects/SensorTile.box.PRO/Applications/SensorStreamer_prj/STM32CubeIDE/STM32U585AIIXQ_FLASH.ld" --specs=nosys.specs -Wl,-Map="SensorTile_box_PRO_SensorStreamer_prj.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

SensorTile_box_PRO_SensorStreamer_prj.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "SensorTile_box_PRO_SensorStreamer_prj.list"
	@echo 'Finished building: $@'
	@echo ' '

SensorTile_box_PRO_SensorStreamer_prj.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "SensorTile_box_PRO_SensorStreamer_prj.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) SensorTile_box_PRO_SensorStreamer_prj.bin SensorTile_box_PRO_SensorStreamer_prj.elf SensorTile_box_PRO_SensorStreamer_prj.list SensorTile_box_PRO_SensorStreamer_prj.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
