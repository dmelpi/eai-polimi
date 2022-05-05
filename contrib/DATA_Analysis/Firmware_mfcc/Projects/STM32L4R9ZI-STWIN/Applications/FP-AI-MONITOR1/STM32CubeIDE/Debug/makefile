################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Middlewares/eLooM/Src/services/subdir.mk
-include Middlewares/eLooM/Src/events/subdir.mk
-include Middlewares/eLooM/Src/drivers/subdir.mk
-include Middlewares/STM32_USB_Device_Library/Core/Src/subdir.mk
-include Middlewares/STM32_USB_Device_Library/Class/CDC/Src/subdir.mk
-include Middlewares/STM32_GenericFFT_Library/Src/subdir.mk
-include Middlewares/FreeRTOS/Source/portable/MemMang/subdir.mk
-include Middlewares/FreeRTOS/Source/portable/GCC/ARM_CM4F/subdir.mk
-include Middlewares/FreeRTOS/Source/subdir.mk
-include Drivers/STM32L4xx_HAL_Driver/Src/Legacy/subdir.mk
-include Drivers/STM32L4xx_HAL_Driver/Src/subdir.mk
-include Drivers/BSP/Components/lps22hh/subdir.mk
-include Drivers/BSP/Components/ism330dhcx/subdir.mk
-include Drivers/BSP/Components/iis3dwb/subdir.mk
-include Drivers/BSP/Components/hts221/subdir.mk
-include Application/mx/Src/subdir.mk
-include Application/lib_features_extraction/Src/subdir.mk
-include Application/gravity_filter/Src/subdir.mk
-include Application/feature_extraction_library/Src/subdir.mk
-include Application/dpu/Src/events/subdir.mk
-include Application/dpu/Src/subdir.mk
-include Application/X-CUBE-AI/App/subdir.mk
-include Application/Startup/subdir.mk
-include Application/Src/services/subdir.mk
-include Application/Src/drivers/subdir.mk
-include Application/Src/subdir.mk
-include Application/SensorManager/Src/services/subdir.mk
-include Application/SensorManager/Src/events/subdir.mk
-include Application/SensorManager/Src/drivers/subdir.mk
-include Application/SensorManager/Src/subdir.mk
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


BUILD_ARTIFACT_NAME := FP-AI-MONITOR1_prj
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
FP-AI-MONITOR1_prj.elf \

MAP_FILES += \
FP-AI-MONITOR1_prj.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
FP-AI-MONITOR1_prj.list \


# All Target
all: main-build

# Main-build Target
main-build: FP-AI-MONITOR1_prj.elf secondary-outputs

# Tool invocations
FP-AI-MONITOR1_prj.elf FP-AI-MONITOR1_prj.map: $(OBJS) $(USER_OBJS) /Users/leonardoiacussi/Desktop/PhD/ST_Motor/MFCC\ -\ JupyterNotebook\ -\ Firmware/Firmware_mfcc/Projects/STM32L4R9ZI-STWIN/Applications/FP-AI-MONITOR1/STM32CubeIDE/STM32L4R9ZIJX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "FP-AI-MONITOR1_prj.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/Users/leonardoiacussi/Desktop/PhD/ST_Motor/MFCC - JupyterNotebook - Firmware/Firmware_mfcc/Projects/STM32L4R9ZI-STWIN/Applications/FP-AI-MONITOR1/STM32CubeIDE/STM32L4R9ZIJX_FLASH.ld" --specs=nosys.specs -Wl,-Map="FP-AI-MONITOR1_prj.map" -Wl,--gc-sections -static -L../../../../../../Drivers/CMSIS/Lib/GCC -L../../../../../../Middlewares/ST/NanoEdge_AI_Library/Lib -L../../../../../../Middlewares/ST/STM32_AI_Library/Lib --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

FP-AI-MONITOR1_prj.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "FP-AI-MONITOR1_prj.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) FP-AI-MONITOR1_prj.elf FP-AI-MONITOR1_prj.list FP-AI-MONITOR1_prj.map default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
