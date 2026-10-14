ROOT_DIR := ../..
TARGET := firmware
BUILD_DIR := $(ROOT_DIR)/build

PREFIX := arm-none-eabi-
CC := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy
SIZE := $(PREFIX)size

MCU := -mcpu=cortex-m3 -mthumb
CFLAGS := $(MCU) -std=c11 -Og -g -Wall -Wextra -ffunction-sections -fdata-sections -MMD -MP
LDFLAGS := $(MCU) -specs=nano.specs -T$(ROOT_DIR)/linker/STM32F103C8Tx_FLASH.ld \
           -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -lc -lm -lnosys

OBJECTS := $(BUILD_DIR)/main.o $(BUILD_DIR)/stm32f10x_startup.o
DEPS := $(OBJECTS:.o=.d)

.DEFAULT_GOAL := all
.PHONY: all clean erase flash help info reset size FORCE

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

$(BUILD_DIR)/main.o: main.c Makefile FORCE

	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/stm32f10x_startup.o: $(ROOT_DIR)/startup/stm32f10x_startup.c Makefile FORCE

	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)

	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SIZE) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf

	$(OBJCOPY) -O ihex $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf

	$(OBJCOPY) -O binary -S $< $@

help:
	@echo "make          Build the Lab 02 firmware."
	@echo "make flash    Build and program the firmware through ST-Link."
	@echo "make info     Show the connected ST-Link and target information."
	@echo "make reset    Reset the connected target."
	@echo "make erase    Erase the target main flash."
	@echo "make size     Show firmware section sizes."
	@echo "make clean    Remove generated build artifacts."

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

FORCE:

-include $(DEPS)
