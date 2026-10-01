# Output file names and build artifact directory.
.DEFAULT_GOAL := all

TARGET := firmware
BUILD_DIR := build

# GNU Arm Embedded Toolchain commands.
PREFIX := arm-none-eabi-
CC := $(PREFIX)gcc
OBJCOPY := $(PREFIX)objcopy
SIZE := $(PREFIX)size

# Cortex-M3 compiler, assembler, and linker options.
MCU := -mcpu=cortex-m3 -mthumb
CFLAGS := $(MCU) -std=c11 -Og -g -Wall -Wextra -ffunction-sections -fdata-sections -MMD -MP
ASFLAGS := $(MCU) -Og -g -x assembler-with-cpp
LDFLAGS := $(MCU) -specs=nano.specs -Tlinker/STM32F103C8Tx_FLASH.ld \
           -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections -lc -lm -lnosys

# Compile application code and the C startup file; no SPL or HAL sources.
C_SOURCES := $(wildcard source/*.c startup/*.c)
S_SOURCES := $(wildcard startup/*.s)
OBJECTS := $(addprefix $(BUILD_DIR)/,$(C_SOURCES:.c=.o)) \
           $(addprefix $(BUILD_DIR)/,$(S_SOURCES:.s=.o))
# Generated header dependencies keep changed headers rebuilding dependents.
DEPS := $(OBJECTS:.o=.d)

.PHONY: all clean erase flash help info reset size

help:
	@echo "Targets:"
	@echo "  make          Build ELF, HEX, and BIN firmware files."
	@echo "  make info     Show the connected ST-Link and target information."
	@echo "  make flash    Build and program firmware.bin at 0x08000000."
	@echo "  make reset    Reset the connected target through ST-Link."
	@echo "  make erase    Erase the target main flash through ST-Link."
	@echo "  make size     Show firmware section sizes."
	@echo "  make clean    Remove generated build artifacts."

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin

# Compile C source while preserving its directory under build/.
$(BUILD_DIR)/%.o: %.c Makefile

	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Keep this rule for optional assembly source files.
$(BUILD_DIR)/%.o: %.s Makefile

	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -c $< -o $@

# Link object files with the STM32F103C8 flash/RAM layout.
$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS)

	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SIZE) $@

# Convert ELF to formats accepted by common flashing tools.
$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf

	$(OBJCOPY) -O ihex $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf

	$(OBJCOPY) -O binary -S $< $@

flash: $(BUILD_DIR)/$(TARGET).bin

	# Program the binary through ST-Link at the flash base address.
	st-flash --reset write $< 0x08000000

# Show ST-Link probe and connected target details.
info:
	st-info --probe

# Reset the target without changing flash contents.
reset:
	st-flash reset

# Erase only the target main flash; option bytes are not changed.
erase:
	st-flash erase

size: $(BUILD_DIR)/$(TARGET).elf

	$(SIZE) -A $<

clean:
	# Remove only generated build artifacts.
	rm -rf $(BUILD_DIR)

# Include compiler-generated header dependency files when they exist.
-include $(DEPS)
