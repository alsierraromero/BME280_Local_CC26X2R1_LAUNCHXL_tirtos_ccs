################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-1138088817:
	@$(MAKE) --no-print-directory -Onone -f subdir_rules.mk build-1138088817-inproc

build-1138088817-inproc: ../BME280.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"C:/ti/ccs1011/xdctools_3_62_00_08_core/xs" --xdcpath="C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/source;C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/kernel/tirtos/packages;" xdc.tools.configuro -o configPkg -t ti.targets.arm.elf.M4F -p ti.platforms.simplelink:CC13X2_CC26X2 -r release -c "C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS" --compileOptions "-mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path=\"C:/Users/Alberto Sierra/workspace_v10/BME280_Local_CC26X2R1_LAUNCHXL_tirtos_ccs\" --include_path=\"C:/Users/Alberto Sierra/workspace_v10/BME280_Local_CC26X2R1_LAUNCHXL_tirtos_ccs/Debug\" --include_path=\"C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/source\" --include_path=\"C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/source/ti/posix/ccs\" --include_path=\"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include\" --define=DeviceFamily_CC26X2 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on  " "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-1138088817 ../BME280.cfg
configPkg/compiler.opt: build-1138088817
configPkg/: build-1138088817

build-1514325089: ../BME280.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/sysconfig_1_8_0/sysconfig_cli.bat" -s "C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/.metadata/product.json" -o "syscfg" --compiler ccs "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_devices_config.c: build-1514325089 ../BME280.syscfg
syscfg/ti_drivers_config.c: build-1514325089
syscfg/ti_drivers_config.h: build-1514325089
syscfg/ti_utils_build_linker.cmd.genlibs: build-1514325089
syscfg/syscfg_c.rov.xs: build-1514325089
syscfg/ti_utils_runtime_model.gv: build-1514325089
syscfg/ti_utils_runtime_Makefile: build-1514325089
syscfg/: build-1514325089

syscfg/%.obj: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/Alberto Sierra/workspace_v10/BME280_Local_CC26X2R1_LAUNCHXL_tirtos_ccs" --include_path="C:/Users/Alberto Sierra/workspace_v10/BME280_Local_CC26X2R1_LAUNCHXL_tirtos_ccs/Debug" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/source" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/source/ti/posix/ccs" --include_path="C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --define=DeviceFamily_CC26X2 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="syscfg/$(basename $(<F)).d_raw" --include_path="C:/Users/Alberto Sierra/workspace_v10/BME280_Local_CC26X2R1_LAUNCHXL_tirtos_ccs/Debug/syscfg" --obj_directory="syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.obj: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/Alberto Sierra/workspace_v10/BME280_Local_CC26X2R1_LAUNCHXL_tirtos_ccs" --include_path="C:/Users/Alberto Sierra/workspace_v10/BME280_Local_CC26X2R1_LAUNCHXL_tirtos_ccs/Debug" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/source" --include_path="C:/ti/simplelink_cc13x2_26x2_sdk_5_10_00_48/source/ti/posix/ccs" --include_path="C:/ti/ccs1011/ccs/tools/compiler/ti-cgt-arm_20.2.5.LTS/include" --define=DeviceFamily_CC26X2 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="$(basename $(<F)).d_raw" --include_path="C:/Users/Alberto Sierra/workspace_v10/BME280_Local_CC26X2R1_LAUNCHXL_tirtos_ccs/Debug/syscfg" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


