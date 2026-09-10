# Trình biên dịch và cờ biên dịch
CC      = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CFLAGS  = -mcpu=cortex-m3 -mthumb -Wall -O0
LDFLAGS = -T linker.ld -nostdlib

# Danh sách file nguồn và file đích
SRCS    = main.c startup.c
TARGET  = main

# Mục tiêu mặc định: build ra file .bin
all: $(TARGET).bin

$(TARGET).elf: $(SRCS) linker.ld
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(SRCS)

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

# Lệnh nạp code trực tiếp vào STM32
flash: $(TARGET).bin
	sudo st-flash --reset write $(TARGET).bin 0x08000000

# Lệnh dọn dẹp các file sinh ra khi build
clean:
	rm -f $(TARGET).elf $(TARGET).bin
