# This is meant to be included in every project's Makefile after ensuring the
# definition and/or inclusion of the following:
#
# PROJECT     : The project name
# OBJS        : Object files meant to be linked to produce the final ELF
# TODO: Should there be a common Makefile defining the following two?
# OPENCM3_DIR : The libopencm3 directory [should be: ../opencm3]
# DEVICE      : The STM32 board name [should be: STM32F407VG]
#
# include $(OPENCM3_DIR)/mk/genlink-config.mk should take care of the rest of
# the variables : LDSCRIPT CPPFLAGS ARCH_FLAGS LIBDEPS LDLIBS
#
# Don't forget to include $(OPENCM3_DIR)/mk/genlink-rules.mk which is having the
# rule to generate $(LDSCRIPT).

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
FLASH = st-flash

USART_CONSOLE ?= USART2
USART_BAUDRATE ?= 115200

CPPFLAGS += -MD -Wall -Wundef -I. -I../include \
            -DUSART_CONSOLE=$(USART_CONSOLE) -DUSART_BAUDRATE=$(USART_BAUDRATE)

CFLAGS = -O0 -std=c99 -ggdb3 $(ARCH_FLAGS) -fno-common -ffunction-sections \
         -fdata-sections -Wextra -Wshadow -Wno-unused-variable \
         -Wimplicit-function-declaration -Wredundant-decls -Wstrict-prototypes \
         -Wmissing-prototypes

LDFLAGS = --static -nostartfiles -T$(LDSCRIPT) $(ARCH_FLAGS) -ggdb3 \
          -Wl,-Map=$(PROJECT).map -Wl,--cref -Wl,--gc-sections \
          -L$(OPENCM3_DIR)/lib

LDLIBS += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

GENERATED_BINS = $(PROJECT).elf $(PROJECT).bin $(PROJECT).map $(OBJS)

all: $(PROJECT).elf

flash: $(PROJECT).bin
	$(FLASH) write $^ 0x8000000

$(PROJECT).elf: $(OBJS) $(LDSCRIPT) $(LIBDEPS)
	$(CC) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f $(GENERATED_BINS) $(LDSCRIPT)

.PHONY: clean flash all
