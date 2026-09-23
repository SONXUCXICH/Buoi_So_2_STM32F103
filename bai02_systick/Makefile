CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
SIZE = arm-none-eabi-size

CFLAGS = -mcpu=cortex-m3 -mthumb -O0 -g
CFLAGS += -Wall
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -DSTM32F10X_MD
CFLAGS += -DUSE_STDPERIPH_DRIVER

CFLAGS += -I.
CFLAGS += -ICMSIS/CM3/CoreSupport
CFLAGS += -ICMSIS/CM3/DeviceSupport/ST/STM32F10x
CFLAGS += -ISTM32F10x_StdPeriph_Driver/inc

LDFLAGS = -mcpu=cortex-m3 -mthumb
LDFLAGS += -Tlinker.ld
LDFLAGS += -nostdlib
LDFLAGS += -Wl,--gc-sections
LDFLAGS += -Wl,-Map=systick.map

all: systick.bin

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

system_stm32f10x.o: system_stm32f10x.c
	$(CC) $(CFLAGS) -c system_stm32f10x.c -o system_stm32f10x.o

gpio.o:
	$(CC) $(CFLAGS) -c STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c -o gpio.o

rcc.o:
	$(CC) $(CFLAGS) -c STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c -o rcc.o

startup.o: startup.s
	$(CC) -mcpu=cortex-m3 -mthumb -c startup.s -o startup.o

systick.elf: main.o system_stm32f10x.o gpio.o rcc.o startup.o
	$(CC) -mcpu=cortex-m3 -mthumb -nostdlib -Tlinker.ld \
	-Wl,--gc-sections \
	-Wl,-Map=systick.map \
	main.o system_stm32f10x.o gpio.o rcc.o startup.o \
	-lgcc -o systick.elf

systick.bin: systick.elf
	$(OBJCOPY) -O binary systick.elf systick.bin
	$(SIZE) systick.elf

clean:
	rm -f *.o *.elf *.bin *.hex *.map
