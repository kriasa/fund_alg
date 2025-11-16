CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -Werror -Iinclude
DEBUG_FLAGS = -g -O0
RELEASE_FLAGS = -O2

TARGET = base_converter
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

SRCS = $(SRC_DIR)/base_conversion.c main.c
OBJS = $(SRCS:%.c=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR) $(TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/$(SRC_DIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -c $< -o $@

debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(BUILD_DIR) $(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

test: all
	./$(TARGET)

.PHONY: all debug clean test