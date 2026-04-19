#include "matrix2d.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

matrix2d* matrix2d_create(int rows, int cols) {
    // Выделяем память под саму структуру-обертку матрицы
    matrix2d *mat = (matrix2d*)malloc(sizeof(matrix2d));
    mat->rows = rows;
    mat->cols = cols;
    
    // В Си двумерный динамический массив создается в два этапа.
    // Этап 1: Выделяем память под массив указателей (каждый указатель — это начало строки).
    mat->data = (float**)malloc(rows * sizeof(float*));
    
    // Этап 2: Выделяем память под сами строки (массивы чисел float).
    for (int i = 0; i < rows; i++) {
        // Используем calloc вместо malloc. 
        // calloc не только выделяет память, но и зануляет её, 
        // избавляя нас от "мусора" в новой матрице.
        mat->data[i] = (float*)calloc(cols, sizeof(float)); 
    }
    
    // Инициализируем вложенную структуру пикселя (требование по встраиванию структур)
    mat->display_pixel = rgbled_create();
    
    return mat;
}

matrix2d* matrix2d_create_default(void) {
    // Удобная функция-обертка для создания матрицы по умолчанию
    return matrix2d_create(3, 3); 
}

matrix2d* matrix2d_copy(const matrix2d *src) {
    if (!src) return NULL; // Защита от нулевого указателя
    
    // Выполняем "глубокое копирование" (deep copy).
    // Мы не просто копируем указатель на старую матрицу, 
    // а выделяем под новую копию независимый блок памяти.
    matrix2d *copy = matrix2d_create(src->rows, src->cols);
    for (int i = 0; i < src->rows; i++) {
        for (int j = 0; j < src->cols; j++) {
            copy->data[i][j] = src->data[i][j]; // Поэлементное копирование данных
        }
    }
    return copy; 
}

void matrix2d_destroy(matrix2d *mat) {
    if (!mat) return;
    
    // Очистка двумерного массива происходит строго в обратном порядке!
    // Шаг 1: Освобождаем каждую строку.
    for (int i = 0; i < mat->rows; i++) {
        free(mat->data[i]);
    }
    // Шаг 2: Освобождаем массив указателей на строки.
    free(mat->data);
    
    // Шаг 3: Очищаем вложенную структуру (важно не забыть, иначе будет утечка).
    rgbled_destroy(mat->display_pixel);
    
    // Шаг 4: Уничтожаем саму структуру матрицы.
    free(mat); 
}

bool matrix2d_eq(const matrix2d *m1, const matrix2d *m2) {
    // Матрицы разных размеров по определению не равны
    if (m1->rows != m2->rows || m1->cols != m2->cols) return false;
    
    for(int i = 0; i < m1->rows; i++) {
        for(int j = 0; j < m1->cols; j++) {
            // ВАЖНО: Числа с плавающей точкой (float/double) нельзя сравнивать через ==
            // из-за особенностей их аппаратного представления в памяти (стандарт IEEE 754).
            // Вместо этого мы проверяем, не превышает ли модуль их разности (fabs) 
            // заданную малую погрешность (эпсилон, в данном случае 0.0001).
            if(fabs(m1->data[i][j] - m2->data[i][j]) > 0.0001) {
                return false;
            }
        }
    }
    return true; 
}

void matrix2d_print(const matrix2d *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            // %6.2f означает: выделить 6 позиций под число, 2 знака после запятой
            printf("%6.2f ", mat->data[i][j]);
        }
        printf("\n");
    } 
}

void matrix2d_fill_random(matrix2d *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            // Генерируем случайные числа от 0.0 до 9.9
            mat->data[i][j] = (float)(rand() % 100) / 10.0f; 
        }
    }
}

void matrix2d_inc(matrix2d *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) {
            mat->data[i][j]++; // Увеличиваем каждый элемент на 1
        }
    } 
}

void matrix2d_set(matrix2d *mat, int r, int c, float val) {
    // Обязательная проверка границ (bounds checking).
    // Защищает от ошибки "Segmentation fault", если пользователь передаст неверные индексы.
    if(r >= 0 && r < mat->rows && c >= 0 && c < mat->cols) {
        mat->data[r][c] = val; 
    }
}

float* matrix2d_get_row(const matrix2d *mat, int row_idx) {
    if(row_idx < 0 || row_idx >= mat->rows) return NULL;
    
    // Соблюдаем принцип инкапсуляции: вместо того чтобы отдавать указатель 
    // на внутреннюю строку матрицы (которую могут случайно испортить), 
    // мы выделяем новую память и отдаем копию строки.
    // ВАЖНО: вызывающая сторона будет обязана сделать free() для этого массива!
    float* row = (float*)malloc(mat->cols * sizeof(float));
    for(int j = 0; j < mat->cols; j++) {
        row[j] = mat->data[row_idx][j];
    }
    return row; 
}

float* matrix2d_get_col(const matrix2d *mat, int col_idx) {
    if(col_idx < 0 || col_idx >= mat->cols) return NULL;
    
    // Возвращаем копию столбца (собираем его по элементам из каждой строки)
    float* col = (float*)malloc(mat->rows * sizeof(float));
    for(int i = 0; i < mat->rows; i++) {
        col[i] = mat->data[i][col_idx];
    }
    return col; 
}

matrix2d* matrix2d_transpose(const matrix2d *mat) {
    // Транспонирование: строки становятся столбцами, а столбцы — строками.
    // Создаем новую матрицу с перевернутыми размерами.
    matrix2d* t = matrix2d_create(mat->cols, mat->rows);
    for(int i = 0; i < mat->rows; i++) {
        for(int j = 0; j < mat->cols; j++) {
            // Отражаем элементы относительно главной диагонали
            t->data[j][i] = mat->data[i][j]; 
        }
    }
    return t; 
}

float matrix2d_determinant(const matrix2d *mat) {
    // Детерминант существует только для квадратных матриц
    if (mat->rows != mat->cols) return 0.0f; 
    
    // Базовый расчет (формула Крамера) для простейшей матрицы 2x2.
    // Для матриц большего размера (3x3, 4x4) потребуется рекурсивное разложение 
    // по минорам или метод Гаусса (сейчас функция работает как заглушка для них).
    if (mat->rows == 2) {
        return mat->data[0][0]*mat->data[1][1] - mat->data[0][1]*mat->data[1][0];
    }
    
    return 0.0f; 
}
