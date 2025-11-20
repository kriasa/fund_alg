#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

char* copy_str(const char* val) {
    if (val == NULL) return NULL;
    char* new_str = (char*)malloc(strlen(val) + 1);
    if (new_str) strcpy(new_str, val);
    return new_str;
}

void delete_str(const char* val) {
    if (val != NULL) free((void*)val); 
}

VECTOR_DEFINE(char*, StrVector) 

void print_vector_content(const StrVector *v) {
    if (v == NULL) return;
    printf("Vector (size: %zu, capacity: %zu): [", StrVector_size(v), StrVector_capacity(v));
    for (size_t i = 0; i < v->size; i++) {
        if (v->data[i] != NULL) {
             printf("\"%s\"", v->data[i]); 
        } else {
             printf("NULL"); 
        }
        if (i < v->size - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

void handle_status(const char* operation, StrVector_Status status) {
    switch (status) {
        case StrVector_SUCCESS:
            printf("%s: Успешно.\n", operation);
            break;
        case StrVector_ERROR_NULL_PTR:
            printf("%s: Ошибка - Нулевой указатель.\n", operation);
            break;
        case StrVector_ERROR_MEMORY:
            printf("%s: Ошибка - Не удалось выделить память.\n", operation);
            break;
        case StrVector_ERROR_INDEX:
            printf("%s: Ошибка - Недопустимый индекс.\n", operation);
            break;
        case StrVector_ERROR_EMPTY:
            printf("%s: Ошибка - Вектор пуст.\n", operation);
            break;
        default:
            printf("%s: Неизвестная ошибка.\n", operation);
            break;
    }
}

int main() {
    StrVector vec;
    StrVector_Status status;
    int choice = -1;
    size_t capacity;
    char input_buffer[100];
    int index;
    char* result_value;

    printf("Введите начальную вместимость вектора: ");
    if (scanf("%zu", &capacity) != 1) {
        printf("Некорректный ввод. Завершение.\n");
        return 1;
    }
    while (getchar() != '\n'); 

    status = StrVector_create(&vec, capacity, copy_str, delete_str);
    handle_status("Создание вектора", status);
    if (status != StrVector_SUCCESS) return 1;

    while (choice != 0) {
        printf("\n--- Меню ---\n");
        printf("1. push_back (Добавить элемент-строку)\n");
        printf("2. delete_at (Удалить по индексу)\n");
        printf("3. get_at (Получить по индексу)\n");
        printf("4. Вывести вектор\n");
        printf("0. Выход и очистка\n");
        printf("Ваш выбор: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Некорректный ввод команды.\n");
            choice = -1;
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
            case 1:
                printf("Введите строку для добавления (макс. 99 символов): ");
                if (fgets(input_buffer, 100, stdin) != NULL) {
                    input_buffer[strcspn(input_buffer, "\n")] = 0;
                    status = StrVector_push_back(&vec, input_buffer);
                    handle_status("push_back", status);
                } else {
                    printf("Некорректный ввод строки.\n");
                }
                break;
            case 2:
                printf("Введите индекс для удаления: ");
                if (scanf("%d", &index) == 1 && index >= 0) {
                    status = StrVector_delete_at(&vec, (size_t)index);
                    handle_status("delete_at", status);
                } else {
                    printf("Некорректный ввод индекса.\n");
                }
                while (getchar() != '\n');
                break;
            case 3:
                printf("Введите индекс для получения: ");
                if (scanf("%d", &index) == 1 && index >= 0) {
                    status = StrVector_get_at(&vec, (size_t)index, &result_value);
                    if (status == StrVector_SUCCESS) {
                        printf("Элемент по индексу %d: \"%s\"\n", index, result_value);
                    }
                    handle_status("get_at", status);
                } else {
                    printf("Некорректный ввод индекса.\n");
                }
                while (getchar() != '\n');
                break;
            case 4:
                print_vector_content(&vec);
                break;
            case 0:
                printf("Завершение работы.\n");
                break;
            default:
                printf("Неизвестная команда. Попробуйте снова.\n");
                break;
        }
    }

    status = StrVector_delete(&vec);
    handle_status("Очистка вектора (delete)", status);

    return 0;
}