CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -O2 -Wno-unused-function

MAIN_INT_SRC = main.c
MAIN_STR_SRC = str_main.c
HEADER = vector.h
DEMO_INT_EXEC = vector_demo
DEMO_STR_EXEC = str_vector_demo

all: $(DEMO_INT_EXEC) $(DEMO_STR_EXEC)

$(DEMO_INT_EXEC): $(MAIN_INT_SRC) $(HEADER)
	$(CC) $(CFLAGS) $(MAIN_INT_SRC) -o $(DEMO_INT_EXEC)

$(DEMO_STR_EXEC): $(MAIN_STR_SRC) $(HEADER)
	$(CC) $(CFLAGS) $(MAIN_STR_SRC) -o $(DEMO_STR_EXEC)

run_int: $(DEMO_INT_EXEC)
	./$(DEMO_INT_EXEC)

run_str: $(DEMO_STR_EXEC)
	./$(DEMO_STR_EXEC)

clean:
	rm -f $(DEMO_INT_EXEC) $(DEMO_STR_EXEC)

.PHONY: all clean run_int run_str