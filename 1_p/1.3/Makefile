TARGET = program

CC = gcc

CFLAGS = -std=c99 -Wall -Wextra -g -MMD

LDFLAGS = -lm
SRC = main.c lab_functions.c output_utils.c
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

all: $(TARGET)

$(TARGET): $(OBJ)
	@echo "Линковка..."
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)
	@echo "Сборка успешна! Исполняемый файл: $(TARGET)"

%.o: %.c
	@echo "Компиляция $<..."
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "Очистка..."
	rm -f $(TARGET) $(OBJ) $(DEP)

-include $(DEP)