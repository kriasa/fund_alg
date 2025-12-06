CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -O2 -lm
TARGET = integral_calculator
SOURCES = main.c integration.c functions.c cli_output.c
OBJECTS = $(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) -lm

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

test: $(TARGET)
	@echo "Test 1: epsilon = 0.001"
	./$(TARGET) 0.001
	@echo "\nTest 2: epsilon = 0.00001"
	./$(TARGET) 0.00001
	@echo "\nTest 3: invalid epsilon"
	./$(TARGET) -0.1
	@echo "\nTest 4: no arguments"
	./$(TARGET)

.PHONY: all clean test