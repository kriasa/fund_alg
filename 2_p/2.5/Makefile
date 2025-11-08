CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Werror -pedantic
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build

SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/file_processor.c $(SRC_DIR)/text_formatter.c

all: $(BUILD_DIR)/main

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/main: $(SOURCES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $(SOURCES) -o $(BUILD_DIR)/main

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean