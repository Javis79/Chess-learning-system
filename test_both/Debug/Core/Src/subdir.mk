################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/ILI9341_GFX.c \
../Core/Src/ILI9341_STM32_Driver.c \
../Core/Src/buttons.c \
../Core/Src/chess_clock.c \
../Core/Src/chess_pieces.c \
../Core/Src/dma.c \
../Core/Src/gpio.c \
../Core/Src/lcd_scheduler.c \
../Core/Src/led_scheduler.c \
../Core/Src/main.c \
../Core/Src/menu.c \
../Core/Src/mux.c \
../Core/Src/spi.c \
../Core/Src/stm32f4xx_hal_msp.c \
../Core/Src/stm32f4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32f4xx.c \
../Core/Src/tim.c \
../Core/Src/usart.c \
../Core/Src/ws2812.c 

OBJS += \
./Core/Src/ILI9341_GFX.o \
./Core/Src/ILI9341_STM32_Driver.o \
./Core/Src/buttons.o \
./Core/Src/chess_clock.o \
./Core/Src/chess_pieces.o \
./Core/Src/dma.o \
./Core/Src/gpio.o \
./Core/Src/lcd_scheduler.o \
./Core/Src/led_scheduler.o \
./Core/Src/main.o \
./Core/Src/menu.o \
./Core/Src/mux.o \
./Core/Src/spi.o \
./Core/Src/stm32f4xx_hal_msp.o \
./Core/Src/stm32f4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32f4xx.o \
./Core/Src/tim.o \
./Core/Src/usart.o \
./Core/Src/ws2812.o 

C_DEPS += \
./Core/Src/ILI9341_GFX.d \
./Core/Src/ILI9341_STM32_Driver.d \
./Core/Src/buttons.d \
./Core/Src/chess_clock.d \
./Core/Src/chess_pieces.d \
./Core/Src/dma.d \
./Core/Src/gpio.d \
./Core/Src/lcd_scheduler.d \
./Core/Src/led_scheduler.d \
./Core/Src/main.d \
./Core/Src/menu.d \
./Core/Src/mux.d \
./Core/Src/spi.d \
./Core/Src/stm32f4xx_hal_msp.d \
./Core/Src/stm32f4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32f4xx.d \
./Core/Src/tim.d \
./Core/Src/usart.d \
./Core/Src/ws2812.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/ILI9341_GFX.cyclo ./Core/Src/ILI9341_GFX.d ./Core/Src/ILI9341_GFX.o ./Core/Src/ILI9341_GFX.su ./Core/Src/ILI9341_STM32_Driver.cyclo ./Core/Src/ILI9341_STM32_Driver.d ./Core/Src/ILI9341_STM32_Driver.o ./Core/Src/ILI9341_STM32_Driver.su ./Core/Src/buttons.cyclo ./Core/Src/buttons.d ./Core/Src/buttons.o ./Core/Src/buttons.su ./Core/Src/chess_clock.cyclo ./Core/Src/chess_clock.d ./Core/Src/chess_clock.o ./Core/Src/chess_clock.su ./Core/Src/chess_pieces.cyclo ./Core/Src/chess_pieces.d ./Core/Src/chess_pieces.o ./Core/Src/chess_pieces.su ./Core/Src/dma.cyclo ./Core/Src/dma.d ./Core/Src/dma.o ./Core/Src/dma.su ./Core/Src/gpio.cyclo ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/lcd_scheduler.cyclo ./Core/Src/lcd_scheduler.d ./Core/Src/lcd_scheduler.o ./Core/Src/lcd_scheduler.su ./Core/Src/led_scheduler.cyclo ./Core/Src/led_scheduler.d ./Core/Src/led_scheduler.o ./Core/Src/led_scheduler.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/menu.cyclo ./Core/Src/menu.d ./Core/Src/menu.o ./Core/Src/menu.su ./Core/Src/mux.cyclo ./Core/Src/mux.d ./Core/Src/mux.o ./Core/Src/mux.su ./Core/Src/spi.cyclo ./Core/Src/spi.d ./Core/Src/spi.o ./Core/Src/spi.su ./Core/Src/stm32f4xx_hal_msp.cyclo ./Core/Src/stm32f4xx_hal_msp.d ./Core/Src/stm32f4xx_hal_msp.o ./Core/Src/stm32f4xx_hal_msp.su ./Core/Src/stm32f4xx_it.cyclo ./Core/Src/stm32f4xx_it.d ./Core/Src/stm32f4xx_it.o ./Core/Src/stm32f4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32f4xx.cyclo ./Core/Src/system_stm32f4xx.d ./Core/Src/system_stm32f4xx.o ./Core/Src/system_stm32f4xx.su ./Core/Src/tim.cyclo ./Core/Src/tim.d ./Core/Src/tim.o ./Core/Src/tim.su ./Core/Src/usart.cyclo ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/ws2812.cyclo ./Core/Src/ws2812.d ./Core/Src/ws2812.o ./Core/Src/ws2812.su

.PHONY: clean-Core-2f-Src

