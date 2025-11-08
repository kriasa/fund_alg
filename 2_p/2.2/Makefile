TARGET = string_demo
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -D_GNU_SOURCE
SRCS = main.c string_lib.c
OBJS = $(SRCS:.c=.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)