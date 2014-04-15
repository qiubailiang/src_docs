################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
cmd/DSP2833x_Headers_nonBIOS.out: F:/baoyujian/src_docs/Xeuns\ 5.30\ CAN-QEP-POS-SPEED/cmd/DSP2833x_Headers_nonBIOS.cmd $(GEN_CMDS)
	@echo 'Building file: $<'
	@echo 'Invoking: Linker'
	@echo 'Flags: -v28 -ml -g --define="_DEBUG" --define="LARGE_MODEL" --diag_warning=225 --float_support=fpu32 --obj_directory="F:/baoyujian/src_docs/Xeuns 5.30 CAN-QEP-POS-SPEED/Debug" -z -m"F:/baoyujian/Xeuns 5.30 CAN-QEP-POS-SPEED/QEP_POS_SPEED.map" --stack_size=1000 --heap_size=1000 --warn_sections -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000/lib" -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000/include" -i"F:/baoyujian/src_docs/Xeuns 5.30 CAN-QEP-POS-SPEED/QEP_POS_SPEED" -i"F:/baoyujian/src_docs/Xeuns 5.30 CAN-QEP-POS-SPEED" --reread_libs --rom_model'
	$(shell echo -v28 -ml -g --define="_DEBUG" --define="LARGE_MODEL" --diag_warning=225 --float_support=fpu32 --obj_directory="F:/baoyujian/src_docs/Xeuns 5.30 CAN-QEP-POS-SPEED/Debug" -z -m"F:/baoyujian/Xeuns 5.30 CAN-QEP-POS-SPEED/QEP_POS_SPEED.map" --stack_size=1000 --heap_size=1000 --warn_sections -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000/lib" -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000/include" -i"F:/baoyujian/src_docs/Xeuns 5.30 CAN-QEP-POS-SPEED/QEP_POS_SPEED" -i"F:/baoyujian/src_docs/Xeuns 5.30 CAN-QEP-POS-SPEED" --reread_libs --rom_model > "ccsLinker.opt")
	$(shell echo $< >> "ccsLinker.opt")
	$(shell echo ../IQmath_fpu32.lib >> "ccsLinker.opt")
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" -@"ccsLinker.opt" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '

cmd/F28335.out: F:/baoyujian/src_docs/Xeuns\ 5.30\ CAN-QEP-POS-SPEED/cmd/F28335.cmd $(GEN_CMDS)
	@echo 'Building file: $<'
	@echo 'Invoking: Linker'
	@echo 'Flags: -v28 -ml -g --define="_DEBUG" --define="LARGE_MODEL" --diag_warning=225 --float_support=fpu32 --obj_directory="F:/baoyujian/src_docs/Xeuns 5.30 CAN-QEP-POS-SPEED/Debug" -z -m"F:/baoyujian/Xeuns 5.30 CAN-QEP-POS-SPEED/QEP_POS_SPEED.map" --stack_size=1000 --heap_size=1000 --warn_sections -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000/lib" -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000/include" -i"F:/baoyujian/src_docs/Xeuns 5.30 CAN-QEP-POS-SPEED/QEP_POS_SPEED" -i"F:/baoyujian/src_docs/Xeuns 5.30 CAN-QEP-POS-SPEED" --reread_libs --rom_model'
	$(shell echo -v28 -ml -g --define="_DEBUG" --define="LARGE_MODEL" --diag_warning=225 --float_support=fpu32 --obj_directory="F:/baoyujian/src_docs/Xeuns 5.30 CAN-QEP-POS-SPEED/Debug" -z -m"F:/baoyujian/Xeuns 5.30 CAN-QEP-POS-SPEED/QEP_POS_SPEED.map" --stack_size=1000 --heap_size=1000 --warn_sections -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000/lib" -i"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000/include" -i"F:/baoyujian/src_docs/Xeuns 5.30 CAN-QEP-POS-SPEED/QEP_POS_SPEED" -i"F:/baoyujian/src_docs/Xeuns 5.30 CAN-QEP-POS-SPEED" --reread_libs --rom_model > "ccsLinker.opt")
	$(shell echo $< >> "ccsLinker.opt")
	$(shell echo ../IQmath_fpu32.lib >> "ccsLinker.opt")
	"C:/Program Files/Texas Instruments/ccsv4/tools/compiler/c2000/bin/cl2000" -@"ccsLinker.opt" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '


