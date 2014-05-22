################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.local

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/ADC.c" \
"../Sources/Accel.c" \
"../Sources/Config.c" \
"../Sources/Control.c" \
"../Sources/I2C.c" \
"../Sources/LineScanCamera.c" \
"../Sources/Motor.c" \
"../Sources/PID.c" \
"../Sources/Servo.c" \
"../Sources/UART.c" \
"../Sources/main.c" \
"../Sources/printf.c" \
"../Sources/sa_mtb.c" \

C_SRCS += \
../Sources/ADC.c \
../Sources/Accel.c \
../Sources/Config.c \
../Sources/Control.c \
../Sources/I2C.c \
../Sources/LineScanCamera.c \
../Sources/Motor.c \
../Sources/PID.c \
../Sources/Servo.c \
../Sources/UART.c \
../Sources/main.c \
../Sources/printf.c \
../Sources/sa_mtb.c \

OBJS += \
./Sources/ADC.o \
./Sources/Accel.o \
./Sources/Config.o \
./Sources/Control.o \
./Sources/I2C.o \
./Sources/LineScanCamera.o \
./Sources/Motor.o \
./Sources/PID.o \
./Sources/Servo.o \
./Sources/UART.o \
./Sources/main.o \
./Sources/printf.o \
./Sources/sa_mtb.o \

C_DEPS += \
./Sources/ADC.d \
./Sources/Accel.d \
./Sources/Config.d \
./Sources/Control.d \
./Sources/I2C.d \
./Sources/LineScanCamera.d \
./Sources/Motor.d \
./Sources/PID.d \
./Sources/Servo.d \
./Sources/UART.d \
./Sources/main.d \
./Sources/printf.d \
./Sources/sa_mtb.d \

OBJS_QUOTED += \
"./Sources/ADC.o" \
"./Sources/Accel.o" \
"./Sources/Config.o" \
"./Sources/Control.o" \
"./Sources/I2C.o" \
"./Sources/LineScanCamera.o" \
"./Sources/Motor.o" \
"./Sources/PID.o" \
"./Sources/Servo.o" \
"./Sources/UART.o" \
"./Sources/main.o" \
"./Sources/printf.o" \
"./Sources/sa_mtb.o" \

C_DEPS_QUOTED += \
"./Sources/ADC.d" \
"./Sources/Accel.d" \
"./Sources/Config.d" \
"./Sources/Control.d" \
"./Sources/I2C.d" \
"./Sources/LineScanCamera.d" \
"./Sources/Motor.d" \
"./Sources/PID.d" \
"./Sources/Servo.d" \
"./Sources/UART.d" \
"./Sources/main.d" \
"./Sources/printf.d" \
"./Sources/sa_mtb.d" \

OBJS_OS_FORMAT += \
./Sources/ADC.o \
./Sources/Accel.o \
./Sources/Config.o \
./Sources/Control.o \
./Sources/I2C.o \
./Sources/LineScanCamera.o \
./Sources/Motor.o \
./Sources/PID.o \
./Sources/Servo.o \
./Sources/UART.o \
./Sources/main.o \
./Sources/printf.o \
./Sources/sa_mtb.o \


# Each subdirectory must supply rules for building sources it contributes
Sources/ADC.o: ../Sources/ADC.c
	@echo 'Building file: $<'
	@echo 'Executing target #4 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/ADC.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/ADC.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Accel.o: ../Sources/Accel.c
	@echo 'Building file: $<'
	@echo 'Executing target #5 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Accel.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Accel.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Config.o: ../Sources/Config.c
	@echo 'Building file: $<'
	@echo 'Executing target #6 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Config.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Config.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Control.o: ../Sources/Control.c
	@echo 'Building file: $<'
	@echo 'Executing target #7 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Control.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Control.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/I2C.o: ../Sources/I2C.c
	@echo 'Building file: $<'
	@echo 'Executing target #8 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/I2C.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/I2C.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/LineScanCamera.o: ../Sources/LineScanCamera.c
	@echo 'Building file: $<'
	@echo 'Executing target #9 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/LineScanCamera.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/LineScanCamera.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Motor.o: ../Sources/Motor.c
	@echo 'Building file: $<'
	@echo 'Executing target #10 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Motor.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Motor.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/PID.o: ../Sources/PID.c
	@echo 'Building file: $<'
	@echo 'Executing target #11 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/PID.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/PID.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/Servo.o: ../Sources/Servo.c
	@echo 'Building file: $<'
	@echo 'Executing target #12 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/Servo.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/Servo.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/UART.o: ../Sources/UART.c
	@echo 'Building file: $<'
	@echo 'Executing target #13 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/UART.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/UART.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.o: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Executing target #14 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/main.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/main.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/printf.o: ../Sources/printf.c
	@echo 'Building file: $<'
	@echo 'Executing target #15 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/printf.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/printf.o"
	@echo 'Finished building: $<'
	@echo ' '

Sources/sa_mtb.o: ../Sources/sa_mtb.c
	@echo 'Building file: $<'
	@echo 'Executing target #16 $<'
	@echo 'Invoking: ARM Ltd Windows GCC C Compiler'
	"$(ARMSourceryDirEnv)/arm-none-eabi-gcc" "$<" @"Sources/sa_mtb.args" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -o"Sources/sa_mtb.o"
	@echo 'Finished building: $<'
	@echo ' '


