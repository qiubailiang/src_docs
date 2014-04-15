################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
IQmath_fpu32.out: D:/ccsproject/IQmath_fpu32.lib $(GEN_CMDS)
	@echo 'Building file: $<'
	@echo 'Invoking: Linker'
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" -v28 -ml -g --define="_DEBUG" --define="LARGE_MODEL" --diag_warning=225 --float_support=fpu32 --obj_directory="D:/ccsproject/Debug" -z -m"I:/Xeuns 5.30 CAN-QEP-POS-SPEED/Debug/QEP_POS_SPEED.map" --stack_size=1000 --heap_size=1000 --warn_sections -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000/lib" -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000/include" -i"F:/baoyujian/Xeuns 5.30 CAN-QEP-POS-SPEED/QEP_POS_SPEED" -i"D:/ccsproject" --reread_libs --rom_model -o "$@" "$<" "../IQmath_fpu32.lib"
	@echo 'Finished building: $<'
	@echo ' '


