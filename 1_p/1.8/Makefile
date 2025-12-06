CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -Iinclude
TARGET = big_int_converter

SRC_DIR = src
INCLUDE_DIR = include

SOURCES = main.c \
          $(SRC_DIR)/functions.c \
          $(SRC_DIR)/error_handler.c

OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

main.c.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/%.c.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
	rm -f main.o $(SRC_DIR)/functions.o $(SRC_DIR)/error_handler.o