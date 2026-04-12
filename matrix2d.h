#ifndef MATRIX2D_H
#define MATRIX2D_H
#include <stdbool.h>
#include "bitstruct.h"

typedef struct {
    float **data;          // Двойной указатель для хранения 2D массива в куче
    int rows;              // Количество строк
    int cols;              // Количество столбцов
    rgbled *display_pixel; // Встроенная структура из ПР№2 (по заданию)
} matrix2d;

matrix2d* matrix2d_create_default(void);
matrix2d* matrix2d_create(int rows, int cols);
matrix2d* matrix2d_copy(const matrix2d *src);
void matrix2d_destroy(matrix2d *mat);

bool matrix2d_eq(const matrix2d *m1, const matrix2d *m2);
void matrix2d_print(const matrix2d *mat);
void matrix2d_fill_random(matrix2d *mat);
void matrix2d_inc(matrix2d *mat);
void matrix2d_set(matrix2d *mat, int r, int c, float val);

float* matrix2d_get_row(const matrix2d *mat, int row_idx);
float* matrix2d_get_col(const matrix2d *mat, int col_idx);
matrix2d* matrix2d_transpose(const matrix2d *mat);
float matrix2d_determinant(const matrix2d *mat);

#endif
