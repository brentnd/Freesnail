################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/system/ARM_SysTick.c" \
"../Sources/system/CrystalClock.c" \
"../Sources/system/arm_cm0.c" \

C_SRCS += \
../Sources/system/ARM_SysTick.c \
../Sources/system/CrystalClock.c \
../Sources/system/arm_cm0.c \

OBJS += \
./Sources/system/ARM_SysTick.o \
./Sources/system/CrystalClock.o \
./Sources/system/arm_cm0.o \

C_DEPS += \
./Sources/system/ARM_SysTick.d \
./Sources/system/CrystalClock.d \
./Sources/system/arm_cm0.d \

OBJS_QUOTED += \
"./Sources/system/ARM_SysTick.o" \
"./Sources/system/CrystalClock.o" \
"./Sources/system/arm_cm0.o" \

C_DEPS_QUOTED += \
"./Sources/system/ARM_SysTick.d" \
"./Sources/system/CrystalClock.d" \
"./Sources/system/arm_cm0.d" \

OBJS_OS_FORMAT += \
./Sources/system/ARM_SysTick.o \
./Sources/system/CrystalClock.o \
./Sources/system/arm_cm0.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/system/ARM_SysTick.o: ../Sources/system/ARM_SysTick.c
	@echo 'Building file: $<'
	@echo 'Executing target #1 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/system/ARM_SysTick.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/system/ARM_SysTick.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/system/CrystalClock.o: ../Sources/system/CrystalClock.c
	@echo 'Building file: $<'
	@echo 'Executing target #2 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/system/CrystalClock.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/system/CrystalClock.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/system/arm_cm0.o: ../Sources/system/arm_cm0.c
	@echo 'Building file: $<'
	@echo 'Executing target #3 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/system/arm_cm0.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/system/arm_cm0.o"
	@echo 'Finished building: $<'
	@echo ' '


