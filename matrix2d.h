#ifndef MATRIX2D_H
#define MATRIX2D_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

// Структура матрицы
typedef struct {
    int32_t** data;     // Двумерный массив целых чисел
    int size;           // Размер матрицы (size x size)
    bool has_diagonal;  // Наличие главной диагонали
} matrix2d;

// Конструкторы
matrix2d* matrix2d_create_default();                    // по умолчанию
matrix2d* matrix2d_create(int size, bool has_diagonal); // с параметрами
matrix2d* matrix2d_copy(const matrix2d* src);           // копирования

// Деструктор
void matrix2d_free(matrix2d* m);

// Сеттеры (изменение данных)
void matrix2d_increment(matrix2d* m);                    // ++
void matrix2d_decrement(matrix2d* m);                    // --
void matrix2d_set_element(matrix2d* m, int row, int col, int32_t value);

// Геттер
int32_t matrix2d_get_element(const matrix2d* m, int row, int col);

// Сравнение матриц (по сумме элементов)
bool matrix2d_equal(const matrix2d* a, const matrix2d* b);
bool matrix2d_not_equal(const matrix2d* a, const matrix2d* b);
bool matrix2d_greater(const matrix2d* a, const matrix2d* b);
bool matrix2d_less(const matrix2d* a, const matrix2d* b);
bool matrix2d_greater_or_equal(const matrix2d* a, const matrix2d* b);
bool matrix2d_less_or_equal(const matrix2d* a, const matrix2d* b);

// Ввод/Вывод
void matrix2d_print(const matrix2d* m);
void matrix2d_input(matrix2d* m);

// Заполнение случайными значениями
void matrix2d_fill_random(matrix2d* m, int32_t min, int32_t max);

// Получение строки/столбца (возвращаем массив, который нужно освободить)
int32_t* matrix2d_get_row(const matrix2d* m, int row);
int32_t* matrix2d_get_col(const matrix2d* m, int col);

// Транспонирование
matrix2d* matrix2d_transpose(const matrix2d* m);

// Определитель (только для 2x2 и 3x3)
int32_t matrix2d_determinant(const matrix2d* m);

// Обратная матрица (только для 2x2, возвращает float матрицу)
matrix2d* matrix2d_inverse(const matrix2d* m);

#endif
