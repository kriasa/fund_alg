#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

int copy_int(const int val) {
    return val;
}

void delete_int(const int val) {
    (void)val;
}

VECTOR_DEFINE(int, IntVector)

void print_vector_content(const IntVector *v) {
    if (v == NULL) return;
    printf("Vector (size: %zu, capacity: %zu): [", IntVector_size(v), IntVector_capacity(v));
    for (size_t i = 0; i < v->size; i++) {
        printf("%d", v->data[i]); 
        if (i < v->size - 1) printf(", ");
    }
    printf("]\n");
}

void handle_status(const char* operation, IntVector_Status status) {
    switch (status) {
        case IntVector_SUCCESS:
            printf("%s: Успешно.\n", operation);
            break;
        case IntVector_ERROR_NULL_PTR:
            printf("%s: Ошибка - Нулевой указатель.\n", operation);
            break;
        case IntVector_ERROR_MEMORY:
            printf("%s: Ошибка - Не удалось выделить память.\n", operation);
            break;
        case IntVector_ERROR_INDEX:
            printf("%s: Ошибка - Недопустимый индекс.\n", operation);
            break;
        case IntVector_ERROR_EMPTY:
            printf("%s: Ошибка - Вектор пуст.\n", operation);
            break;
        default:
            printf("%s: Неизвестная ошибка.\n", operation);
            break;
    }
}

int main() {
    IntVector vec;
    IntVector_Status status;
    int choice = -1;
    size_t capacity;
    int input_value;
    int index;
    int result_value;

    printf("Введите начальную вместимость вектора: ");
    if (scanf("%zu", &capacity) != 1) {
        printf("Некорректный ввод. Завершение.\n");
        return 1;
    }
    while (getchar() != '\n'); 

    status = IntVector_create(&vec, capacity, copy_int, delete_int);
    handle_status("Создание вектора", status);
    if (status != IntVector_SUCCESS) return 1;

    while (choice != 0) {
        printf("\n--- Меню ---\n");
        printf("1. push_back (Добавить элемент)\n");
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
                printf("Введите значение (int) для добавления: ");
                if (scanf("%d", &input_value) == 1) {
                    status = IntVector_push_back(&vec, input_value);
                    handle_status("push_back", status);
                } else {
                    printf("Некорректный ввод числа.\n");
                }
                while (getchar() != '\n');
                break;
            case 2:
                printf("Введите индекс для удаления: ");
                if (scanf("%d", &index) == 1 && index >= 0) {
                    status = IntVector_delete_at(&vec, (size_t)index);
                    handle_status("delete_at", status);
                } else {
                    printf("Некорректный ввод индекса.\n");
                }
                while (getchar() != '\n');
                break;
            case 3:
                printf("Введите индекс для получения: ");
                if (scanf("%d", &index) == 1 && index >= 0) {
                    status = IntVector_get_at(&vec, (size_t)index, &result_value);
                    if (status == IntVector_SUCCESS) {
                        printf("Элемент по индексу %d: %d\n", index, result_value);
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

    status = IntVector_delete(&vec);
    handle_status("Очистка вектора (delete)", status);

    return 0;
}