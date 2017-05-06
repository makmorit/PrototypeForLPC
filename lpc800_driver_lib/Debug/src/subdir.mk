################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lpc8xx_bod.c \
../src/lpc8xx_clkconfig.c \
../src/lpc8xx_comp.c \
../src/lpc8xx_crc.c \
../src/lpc8xx_gpio.c \
../src/lpc8xx_i2c.c \
../src/lpc8xx_mrt.c \
../src/lpc8xx_nmi.c \
../src/lpc8xx_pmu.c \
../src/lpc8xx_spi.c \
../src/lpc8xx_uart.c \
../src/lpc8xx_wdt.c \
../src/lpc8xx_wkt.c 

OBJS += \
./src/lpc8xx_bod.o \
./src/lpc8xx_clkconfig.o \
./src/lpc8xx_comp.o \
./src/lpc8xx_crc.o \
./src/lpc8xx_gpio.o \
./src/lpc8xx_i2c.o \
./src/lpc8xx_mrt.o \
./src/lpc8xx_nmi.o \
./src/lpc8xx_pmu.o \
./src/lpc8xx_spi.o \
./src/lpc8xx_uart.o \
./src/lpc8xx_wdt.o \
./src/lpc8xx_wkt.o 

C_DEPS += \
./src/lpc8xx_bod.d \
./src/lpc8xx_clkconfig.d \
./src/lpc8xx_comp.d \
./src/lpc8xx_crc.d \
./src/lpc8xx_gpio.d \
./src/lpc8xx_i2c.d \
./src/lpc8xx_mrt.d \
./src/lpc8xx_nmi.d \
./src/lpc8xx_pmu.d \
./src/lpc8xx_spi.d \
./src/lpc8xx_uart.d \
./src/lpc8xx_wdt.d \
./src/lpc8xx_wkt.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSIS_CORE_LPC8xx -D__LPC8XX__ -I"/Users/makmorit/GitHub/PrototypeForLPC/lpc800_driver_lib/inc" -I"/Users/makmorit/GitHub/PrototypeForLPC/CMSIS_CORE_LPC8xx/inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m0 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


