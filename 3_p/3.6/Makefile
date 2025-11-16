CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -Werror -O2 -Iinclude
TARGET = brackets_checker
SOURCES = main.c src/brackets.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean