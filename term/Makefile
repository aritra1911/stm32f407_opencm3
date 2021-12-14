PROJECT = $(lastword $(subst /, ,$(CURDIR)))
SRCS = main.c
OBJS = $(SRCS:%.c=%.o)

OPENCM3_DIR = ../libopencm3
DEVICE ?= STM32F407VG

include $(OPENCM3_DIR)/mk/genlink-config.mk
include ../rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
