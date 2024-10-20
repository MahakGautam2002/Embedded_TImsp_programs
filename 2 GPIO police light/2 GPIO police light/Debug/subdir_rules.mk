################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"E:/KPIT/2 GPIO police light" -I"E:/KPIT/2 GPIO police light/Debug" -I"C:/ti/mspm0_sdk_2_02_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_02_00_05/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-158207167: ../empty.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/sysconfig_1.21.0/sysconfig_cli.bat" --script "E:/KPIT/2 GPIO police light/empty.syscfg" -o "." -s "C:/ti/mspm0_sdk_2_02_00_05/.metadata/product.json" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

device_linker.cmd: build-158207167 ../empty.syscfg
device.opt: build-158207167
device.cmd.genlibs: build-158207167
ti_msp_dl_config.c: build-158207167
ti_msp_dl_config.h: build-158207167
Event.dot: build-158207167

%.o: ./%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"E:/KPIT/2 GPIO police light" -I"E:/KPIT/2 GPIO police light/Debug" -I"C:/ti/mspm0_sdk_2_02_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_02_00_05/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

startup_mspm0g350x_ticlang.o: C:/ti/mspm0_sdk_2_02_00_05/source/ti/devices/msp/m0p/startup_system_files/ticlang/startup_mspm0g350x_ticlang.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1271/ccs/tools/compiler/ti-cgt-armllvm_3.2.2.LTS/bin/tiarmclang.exe" -c @"device.opt"  -march=thumbv6m -mcpu=cortex-m0plus -mfloat-abi=soft -mlittle-endian -mthumb -O2 -I"E:/KPIT/2 GPIO police light" -I"E:/KPIT/2 GPIO police light/Debug" -I"C:/ti/mspm0_sdk_2_02_00_05/source/third_party/CMSIS/Core/Include" -I"C:/ti/mspm0_sdk_2_02_00_05/source" -gdwarf-3 -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


