
INC += $(SIMBA_ROOT)/src/boards/B52A
SRC += $(SIMBA_ROOT)/src/boards/B52A/board.c

LINKER_SCRIPT ?= simba.flash.ld
ESP_FLASH_SIZE = 4M

ifeq ($(PARTITION), bootloader)
PARTITIONS_BIN ?= $(SIMBA_ROOT)/3pp/esp32/bin/partitions_bootloader_application.bin
erase: all
upload: erase
else
PARTITIONS_BIN ?= $(SIMBA_ROOT)/3pp/esp32/bin/partitions_singleapp.bin
endif

BOARD_HOMEPAGE = "http://esp32.de"
BOARD_PINOUT = "nano32-pinout.jpg"
BOARD_DESC = "B52A"

MCU = esp32
SERIAL_PORT ?= /dev/arduino
CONSOLE_RESET_TYPE ?= 0
TIMEOUT ?= 10
BAUDRATE ?= 115200

upload:
	@echo "Uploading $(EXE)."
	python -u $(SIMBA_ROOT)/3pp/esp32/esp-idf/components/esptool_py/esptool/esptool.py \
	--chip esp32 --port $(SERIAL_PORT) --baud 921600 write_flash -z \
	--flash_mode dio --flash_freq 40m --flash_size 4MB \
	0x1000 $(SIMBA_ROOT)/3pp/esp32/bin/bootloader.bin \
	0x10000 $(BIN) \
	0x8000 $(PARTITIONS_BIN)

rerun:
	@echo "Running '$(EXE)'."
	python -u $(RUN_PY) --port $(SERIAL_PORT) \
			    --timeout $(TIMEOUT) \
			    --baudrate $(BAUDRATE) \
			    --pattern $(RUN_END_PATTERN)\
			    --pattern-success $(RUN_END_PATTERN_SUCCESS) \
			    | tee $(RUNLOG) ; test $${PIPESTATUS[0]} -eq 0

erase:
	@echo "Erasing."
	python -u $(ESPTOOL_PY) \
	--chip esp32 --port $(SERIAL_PORT) --baud 921600 erase_flash
