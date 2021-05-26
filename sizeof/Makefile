PROJECT = $(lastword $(subst /, ,$(CURDIR)))
SRCS = sizeof.c ../usart_printf/usart_printf.c
OBJS = $(SRCS:%.c=%.o)

# You'll need to perform a `make clean` after altering these
USART_CONSOLE := USART2
USART_BAUDRATE := 115200

OPENCM3_DIR = ../libopencm3
DEVICE = STM32F407VG

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
