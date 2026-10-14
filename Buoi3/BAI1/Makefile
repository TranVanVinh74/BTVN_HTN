.DEFAULT_GOAL := all

ROOT_DIR := .
TARGET := firmware
BUILD_DIR := $(ROOT_DIR)/build

PREFIX := arm-none-eabi-
CC := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy
SIZE := $(PREFIX)size

SPL_DIR := $(ROOT_DIR)/drivers/STM32F10x_StdPeriph_Driver
CPPFLAGS := -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 \
            -I$(ROOT_DIR)/include \
            -I$(ROOT_DIR)/drivers/CMSIS/Include \
            -I$(ROOT_DIR)/drivers/CMSIS/Device/ST/STM32F1xx/Include \
            -I$(SPL_DIR)/inc
MCU := -mcpu=cortex-m3 -mthumb
CFLAGS := $(MCU) -std=c11 -Og -g -Wall -Wextra -ffunction-sections -fdata-sections -MMD -MP
ASFLAGS := $(MCU) -g -x assembler-with-cpp
LDFLAGS := $(MCU) -specs=nano.specs -T$(ROOT_DIR)/linker/STM32F103C8Tx_FLASH.ld \
           -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -lc -lm -lnosys

SPL_SOURCES := $(wildcard $(SPL_DIR)/src/*.c)
SPL_OBJECTS := $(patsubst $(SPL_DIR)/src/%.c,$(BUILD_DIR)/spl/%.o,$(SPL_SOURCES))
OBJECTS := $(BUILD_DIR)/main.o $(BUILD_DIR)/ssd1306.o $(BUILD_DIR)/system_stm32f10x.o \
           $(BUILD_DIR)/startup_stm32f103xb.o $(SPL_OBJECTS)
DEPS := $(OBJECTS:.o=.d)

.PHONY: all clean erase flash help info reset size

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/main.o: main.c Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/ssd1306.o: ssd1306.c Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/system_stm32f10x.o: $(ROOT_DIR)/source/system_stm32f10x.c Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/startup_stm32f103xb.o: $(ROOT_DIR)/startup/startup_stm32f103xb.s Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR)/spl/%.o: $(SPL_DIR)/src/%.c Makefile
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SIZE) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -O ihex $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf
	$(OBJCOPY) -O binary -S $< $@

flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash --reset write $< 0x08000000

info:
	st-info --probe

reset:
	st-flash reset

erase:
	st-flash erase

size: $(BUILD_DIR)/$(TARGET).elf
	$(SIZE) -A $<

clean:
	rm -rf $(BUILD_DIR)

help:
	@echo "make        Build firmware"
	@echo "make flash  Build and flash through ST-Link"

-include $(DEPS)
