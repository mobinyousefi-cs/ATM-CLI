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
        $(SRC_DIR)/ui.c

OBJS := $(SRCS:.c=.o)

.PHONY: all clean debug release

all: $(TARGET)

debug: CFLAGS += -g -O0
debug: clean all

release: CFLAGS += -O2
release: clean all

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/%.h $(INC_DIR)/common.h
	$(CC) $(CFLAGS) -c -o $@ $<

# Special rules for main.c (no corresponding header) and ui.c (depends on ui.h)
$(SRC_DIR)/main.o: $(SRC_DIR)/main.c $(INC_DIR)/atm.h $(INC_DIR)/common.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(SRC_DIR)/ui.o: $(SRC_DIR)/ui.c $(INC_DIR)/ui.h $(INC_DIR)/common.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJS) $(TARGET)
