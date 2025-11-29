# Project:   Command-Line ATM Interface
# Author:    Mobin Yousefi (GitHub: github.com/mobinyousefi-cs)
# License:   MIT

CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -pedantic -Iinclude
LDFLAGS := 
TARGET  := atm_cli

SRC_DIR := src
INC_DIR := include

SRCS := $(SRC_DIR)/main.c \
        $(SRC_DIR)/atm.c  \
        $(SRC_DIR)/account.c \
        $(SRC_DIR)/auth.c \
        $(SRC_DIR)/ui.c \
        $(SRC_DIR)/db_json.c

OBJS := $(SRCS:.c=.o)

.PHONY: all clean debug release

all: $(TARGET)

debug: CFLAGS += -g -O0
debug: clean all

release: CFLAGS += -O2
release: clean all

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS) $(TARGET)
