.RECIPEPREFIX := >

TARGET = pwm

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CPU = -mcpu=cortex-m3 -mthumb

CFLAGS = $(CPU)
CFLAGS += -O0
CFLAGS += -g
CFLAGS += -Wall
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -DSTM32F10X_MD
CFLAGS += -DUSE_STDPERIPH_DRIVER

CFLAGS += -I.
CFLAGS += -ICMSIS/CM3/CoreSupport
CFLAGS += -ICMSIS/CM3/DeviceSupport/ST/STM32F10x
CFLAGS += -ISTM32F10x_StdPeriph_Driver/inc


OBJS = main.o
OBJS += system_stm32f10x.o
OBJS += gpio.o
OBJS += rcc.o
OBJS += tim.o
OBJS += startup.o


all: $(TARGET).bin


main.o: main.c
>$(CC) $(CFLAGS) -c main.c -o main.o


system_stm32f10x.o: system_stm32f10x.c
>$(CC) $(CFLAGS) -c system_stm32f10x.c -o system_stm32f10x.o


gpio.o: STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c
>$(CC) $(CFLAGS) -c STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c -o gpio.o


rcc.o: STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c
>$(CC) $(CFLAGS) -c STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c -o rcc.o


tim.o: STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c
>$(CC) $(CFLAGS) -c STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c -o tim.o


startup.o: startup.s
>$(CC) $(CPU) -c startup.s -o startup.o


$(TARGET).elf: $(OBJS)
>$(CC) $(CPU) -nostdlib -Tlinker.ld \
-Wl,--gc-sections \
-Wl,-Map=$(TARGET).map \
$(OBJS) \
-lgcc \
-o $(TARGET).elf


$(TARGET).bin: $(TARGET).elf
>$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin
>$(SIZE) $(TARGET).elf


clean:
>rm -f *.o *.elf *.bin *.hex *.map


flash: $(TARGET).bin
>st-flash write $(TARGET).bin 0x08000000