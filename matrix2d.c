#include "matrix2d.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

matrix2d* matrix2d_create(int rows, int cols) {
    matrix2d *mat = (matrix2d*)malloc(sizeof(matrix2d));
    mat->rows = rows;
    mat->cols = cols;
    
    // Выделяем память под массив указателей на строки
    mat->data = (float**)malloc(rows * sizeof(float*));
    for (int i = 0; i < rows; i++) {
        // calloc зануляет память при выделении
        mat->data[i] = (float*)calloc(cols, sizeof(float)); 
    }
    
    // Инициализируем вложенную структуру
    mat->display_pixel = rgbled_create();
    return mat;
}

matrix2d* matrix2d_create_default(void) {
    return matrix2d_create(3, 3); // Матрица 3x3 по умолчанию
}

matrix2d* matrix2d_copy(const matrix2d *src) {
    if (!src) return NULL;
    matrix2d *copy = matrix2d_create(src->rows, src->cols);
    for (int i = 0; i < src->rows; i++)
        for (int j = 0; j < src->cols; j++)
            copy->data[i][j] = src->data[i][j]; // Поэлементное копирование
    return copy; 
}

void matrix2d_destroy(matrix2d *mat) {
    if (!mat) return;
    // Сначала очищаем строки
    for (int i = 0; i < mat->rows; i++) free(mat->data[i]);
    // Затем массив указателей
    free(mat->data);
    // Очищаем вложенную структуру
    rgbled_destroy(mat->display_pixel);
    free(mat); 
}

bool matrix2d_eq(const matrix2d *m1, const matrix2d *m2) {
    if (m1->rows != m2->rows || m1->cols != m2->cols) return false;
    for(int i = 0; i < m1->rows; i++)
        for(int j = 0; j < m1->cols; j++)
            // Для float используем сравнение с погрешностью (эпсилон)
            if(fabs(m1->data[i][j] - m2->data[i][j]) > 0.0001) return false;
    return true; 
}

void matrix2d_print(const matrix2d *mat) {
    for (int i = 0; i < mat->rows; i++) {
        for (int j = 0; j < mat->cols; j++) 
            printf("%6.2f ", mat->data[i][j]);
        printf("\n");
    } 
}

void matrix2d_fill_random(matrix2d *mat) {
    for (int i = 0; i < mat->rows; i++)
        for (int j = 0; j < mat->cols; j++)
            mat->data[i][j] = (float)(rand() % 100) / 10.0f; // Числа от 0.0 до 9.9
}

void matrix2d_inc(matrix2d *mat) {
    for (int i = 0; i < mat->rows; i++)
        for (int j = 0; j < mat->cols; j++) mat->data[i][j]++; 
}

void matrix2d_set(matrix2d *mat, int r, int c, float val) {
    // Защита от выхода за границы массива
    if(r >= 0 && r < mat->rows && c >= 0 && c < mat->cols) 
        mat->data[r][c] = val; 
}

float* matrix2d_get_row(const matrix2d *mat, int row_idx) {
    if(row_idx < 0 || row_idx >= mat->rows) return NULL;
    // Возвращаем копию строки, чтобы не нарушать инкапсуляцию
    float* row = (float*)malloc(mat->cols * sizeof(float));
    for(int j = 0; j < mat->cols; j++) row[j] = mat->data[row_idx][j];
    return row; 
}

float* matrix2d_get_col(const matrix2d *mat, int col_idx) {
    if(col_idx < 0 || col_idx >= mat->cols) return NULL;
    float* col = (float*)malloc(mat->rows * sizeof(float));
    for(int i = 0; i < mat->rows; i++) col[i] = mat->data[i][col_idx];
    return col; 
}

matrix2d* matrix2d_transpose(const matrix2d *mat) {
    // Меняем местами размеры (rows <-> cols)
    matrix2d* t = matrix2d_create(mat->cols, mat->rows);
    for(int i = 0; i < mat->rows; i++)
        for(int j = 0; j < mat->cols; j++)
            t->data[j][i] = mat->data[i][j]; // Отражение относительно главной диагонали
    return t; 
}

float matrix2d_determinant(const matrix2d *mat) {
    if (mat->rows != mat->cols) return 0.0f; // Детерминант только для квадратных матриц
    // Базовый расчет для матрицы 2x2
    if (mat->rows == 2) 
        return mat->data[0][0]*mat->data[1][1] - mat->data[0][1]*mat->data[1][0];
    return 0.0f; 
}
