################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CFG_SRCS += \
../BME280.cfg 

CMD_SRCS += \
../cc13x2_cc26x2_tirtos.cmd 

SYSCFG_SRCS += \
../BME280.syscfg 

C_SRCS += \
./syscfg/ti_devices_config.c \
./syscfg/ti_drivers_config.c \
../bme280.c \
../main.c 

GEN_CMDS += \
./configPkg/linker.cmd 

GEN_FILES += \
./configPkg/linker.cmd \
./configPkg/compiler.opt \
./syscfg/ti_devices_config.c \
./syscfg/ti_drivers_config.c 

GEN_MISC_DIRS += \
./configPkg/ \
./syscfg/ 

C_DEPS += \
./syscfg/ti_devices_config.d \
./syscfg/ti_drivers_config.d \
./bme280.d \
./main.d 

GEN_OPTS += \
./configPkg/compiler.opt 

OBJS += \
./syscfg/ti_devices_config.obj \
./syscfg/ti_drivers_config.obj \
./bme280.obj \
./main.obj 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/ti_utils_build_linker.cmd.genlibs \
./syscfg/syscfg_c.rov.xs \
./syscfg/ti_utils_runtime_model.gv \
./syscfg/ti_utils_runtime_Makefile 

GEN_MISC_DIRS__QUOTED += \
"configPkg\" \
"syscfg\" 

OBJS__QUOTED += \
"syscfg\ti_devices_config.obj" \
"syscfg\ti_drivers_config.obj" \
"bme280.obj" \
"main.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_drivers_config.h" \
"syscfg\ti_utils_build_linker.cmd.genlibs" \
"syscfg\syscfg_c.rov.xs" \
"syscfg\ti_utils_runtime_model.gv" \
"syscfg\ti_utils_runtime_Makefile" 

C_DEPS__QUOTED += \
"syscfg\ti_devices_config.d" \
"syscfg\ti_drivers_config.d" \
"bme280.d" \
"main.d" 

GEN_FILES__QUOTED += \
"configPkg\linker.cmd" \
"configPkg\compiler.opt" \
"syscfg\ti_devices_config.c" \
"syscfg\ti_drivers_config.c" 

SYSCFG_SRCS__QUOTED += \
"../BME280.syscfg" 

C_SRCS__QUOTED += \
"./syscfg/ti_devices_config.c" \
"./syscfg/ti_drivers_config.c" \
"../bme280.c" \
"../main.c" 


