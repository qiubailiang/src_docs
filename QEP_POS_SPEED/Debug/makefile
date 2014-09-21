################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := D:/CCS/ccsv5/tools/compiler/c2000_6.2.0

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"../cmd/F28335.cmd" \
"F:/Xeuns 1116 CAN-QEP-POS-SPEED/Debug/Example_posspeed.obj" \
"F:/Xeuns 1116 CAN-QEP-POS-SPEED/Debug/Example_EPwmSetup.obj" \
"F:/Xeuns 1116 CAN-QEP-POS-SPEED/Debug/Example_2833xEqep_pos_speed.obj" \
"F:/Xeuns 1116 CAN-QEP-POS-SPEED/Debug/DSP2833x_usDelay.obj" \
"F:/Xeuns 1116 CAN-QEP-POS-SPEED/Debug/DSP2833x_Xintf.obj" \
"F:/Xeuns 1116 CAN-QEP-POS-SPEED/Debug/DSP2833x_SysCtrl.obj" \
"F:/Xeuns 1116 CAN-QEP-POS-SPEED/Debug/DSP2833x_Spi.obj" \
"F:/Xeuns 1116 CAN-QEP-POS-SPEED/Debug/DSP2833x_PieVect.obj" \
"F:/Xeuns 1116 CAN-QEP-POS-SPEED/Debug/DSP2833x_PieCtrl.obj" \
"F:/Xeuns 1116 CAN-QEP-POS-SPEED/Debug/DSP2833x_GlobalVariableDefs.obj" \
"F:/Xeuns 1116 CAN-QEP-POS-SPEED/Debug/DSP2833x_EQep.obj" \
"F:/Xeuns 1116 CAN-QEP-POS-SPEED/Debug/DSP2833x_EPwm.obj" \
"F:/Xeuns 1116 CAN-QEP-POS-SPEED/Debug/DSP2833x_ECan.obj" \
"F:/Xeuns 1116 CAN-QEP-POS-SPEED/Debug/DSP2833x_DefaultIsr.obj" \
"F:/Xeuns 1116 CAN-QEP-POS-SPEED/Debug/DSP2833x_CpuTimers.obj" \
"F:/Xeuns 1116 CAN-QEP-POS-SPEED/Debug/DSP2833x_CodeStartBranch.obj" \
"F:/Xeuns 1116 CAN-QEP-POS-SPEED/Debug/DSP2833x_ADC_cal.obj" \
"../IQmath_fpu32.lib" \
"../source/IQmath_fpu32.lib" \
"../cmd/DSP2833x_Headers_nonBIOS.cmd" \
-l"rts2800_fpu32.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include source/subdir_vars.mk
-include cmd/subdir_vars.mk
-include subdir_rules.mk
-include source/subdir_rules.mk
-include cmd/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: D:/baoyujian/QEP_POS_SPEED/Debug/QEP_POS_SPEED.out

# Tool invocations
D:/baoyujian/QEP_POS_SPEED/Debug/QEP_POS_SPEED.out: $(OBJS) $(CMD_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C2000 Linker'
	"D:/CCS/ccsv5/tools/compiler/c2000_6.2.0/bin/cl2000" -v28 -ml --float_support=fpu32 -g --define="_DEBUG" --define="LARGE_MODEL" --diag_warning=225 --obj_directory="F:/Xeuns 1116 CAN-QEP-POS-SPEED/Debug" -z -m"D:/baoyujian/QEP_POS_SPEED/Debug/QEP_POS_SPEED.map" --stack_size=1000 --heap_size=1000 --warn_sections -i"D:/CCS/ccsv5/tools/compiler/c2000_6.2.0/lib" -i"D:/CCS/ccsv5/tools/compiler/c2000_6.2.0/include" -i"F:/src_docs/QEP_POS_SPEED" -i"F:/Xeuns 1116 CAN-QEP-POS-SPEED" --reread_libs --xml_link_info="D:/baoyujian/QEP_POS_SPEED/Debug/QEP_POS_SPEED_linkInfo.xml" --rom_model -o "D:/baoyujian/QEP_POS_SPEED/Debug/QEP_POS_SPEED.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C2000_EXECUTABLE_OUTPUTS__QUOTED) "D:\baoyujian\QEP_POS_SPEED\Debug\QEP_POS_SPEED.out"
	-$(RM) "source\DSP2833x_CpuTimers.pp" "source\DSP2833x_DefaultIsr.pp" "source\DSP2833x_ECan.pp" "source\DSP2833x_EPwm.pp" "source\DSP2833x_EQep.pp" "source\DSP2833x_GlobalVariableDefs.pp" "source\DSP2833x_PieCtrl.pp" "source\DSP2833x_PieVect.pp" "source\DSP2833x_Spi.pp" "source\DSP2833x_SysCtrl.pp" "source\DSP2833x_Xintf.pp" "source\Example_2833xEqep_pos_speed.pp" "source\Example_EPwmSetup.pp" "source\Example_posspeed.pp" 
	-$(RM) "F:\Xeuns 1116 CAN-QEP-POS-SPEED\Debug\DSP2833x_ADC_cal.obj" "F:\Xeuns 1116 CAN-QEP-POS-SPEED\Debug\DSP2833x_CodeStartBranch.obj" "F:\Xeuns 1116 CAN-QEP-POS-SPEED\Debug\DSP2833x_CpuTimers.obj" "F:\Xeuns 1116 CAN-QEP-POS-SPEED\Debug\DSP2833x_DefaultIsr.obj" "F:\Xeuns 1116 CAN-QEP-POS-SPEED\Debug\DSP2833x_ECan.obj" "F:\Xeuns 1116 CAN-QEP-POS-SPEED\Debug\DSP2833x_EPwm.obj" "F:\Xeuns 1116 CAN-QEP-POS-SPEED\Debug\DSP2833x_EQep.obj" "F:\Xeuns 1116 CAN-QEP-POS-SPEED\Debug\DSP2833x_GlobalVariableDefs.obj" "F:\Xeuns 1116 CAN-QEP-POS-SPEED\Debug\DSP2833x_PieCtrl.obj" "F:\Xeuns 1116 CAN-QEP-POS-SPEED\Debug\DSP2833x_PieVect.obj" "F:\Xeuns 1116 CAN-QEP-POS-SPEED\Debug\DSP2833x_Spi.obj" "F:\Xeuns 1116 CAN-QEP-POS-SPEED\Debug\DSP2833x_SysCtrl.obj" "F:\Xeuns 1116 CAN-QEP-POS-SPEED\Debug\DSP2833x_Xintf.obj" "F:\Xeuns 1116 CAN-QEP-POS-SPEED\Debug\DSP2833x_usDelay.obj" "F:\Xeuns 1116 CAN-QEP-POS-SPEED\Debug\Example_2833xEqep_pos_speed.obj" "F:\Xeuns 1116 CAN-QEP-POS-SPEED\Debug\Example_EPwmSetup.obj" "F:\Xeuns 1116 CAN-QEP-POS-SPEED\Debug\Example_posspeed.obj" 
	-$(RM) "source\DSP2833x_ADC_cal.pp" "source\DSP2833x_CodeStartBranch.pp" "source\DSP2833x_usDelay.pp" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

