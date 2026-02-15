#include "matrix2d.h"
#include <stdio.h>

int main() {
    printf("=== ДЕМОНСТРАЦИЯ РАБОТЫ С МАТРИЦАМИ ===\n\n");
    
    // 1. Конструктор по умолчанию
    printf("1. Создание матрицы через конструктор по умолчанию:\n");
    matrix2d* m1 = matrix2d_create_default();
    matrix2d_print(m1);
    
    // 2. Конструктор с параметрами
    printf("\n2. Создание матрицы 3x3:\n");
    matrix2d* m2 = matrix2d_create(3, true);
    matrix2d_fill_random(m2, 1, 9);
    matrix2d_print(m2);
    
    // 3. Конструктор копирования
    printf("\n3. Копирование матрицы:\n");
    matrix2d* m3 = matrix2d_copy(m2);
    matrix2d_print(m3);
    
    // 4. Изменение данных
    printf("\n4. Изменение данных:\n");
    printf("Инкремент (++):\n");
    matrix2d_increment(m2);
    matrix2d_print(m2);
    
    printf("Декремент (--):\n");
    matrix2d_decrement(m2);
    matrix2d_print(m2);
    
    printf("Установка элемента [0][0] = 100:\n");
    matrix2d_set_element(m2, 0, 0, 100);
    matrix2d_print(m2);
    
    // 5. Сравнение
    printf("\n5. Сравнение матриц:\n");
    printf("m2 == m3? %s\n", matrix2d_equal(m2, m3) ? "да" : "нет");
    printf("m2 > m3? %s\n", matrix2d_greater(m2, m3) ? "да" : "нет");
    
    // 6. Получение строки и столбца
    printf("\n6. Получение строки и столбца:\n");
    int* row = matrix2d_get_row(m2, 0);
    printf("Строка 0: ");
    for (int i = 0; i < m2->size; i++) {
        printf("%d ", row[i]);
    }
    printf("\n");
    free(row);
    
    int* col = matrix2d_get_col(m2, 0);
    printf("Столбец 0: ");
    for (int i = 0; i < m2->size; i++) {
        printf("%d ", col[i]);
    }
    printf("\n");
    free(col);
    
    // 7. Транспонирование
    printf("\n7. Транспонирование:\n");
    matrix2d* m4 = matrix2d_transpose(m2);
    printf("Исходная:\n");
    matrix2d_print(m2);
    printf("Транспонированная:\n");
    matrix2d_print(m4);
    
    // 8. Определитель
    printf("\n8. Определитель:\n");
    printf("Определитель = %d\n", matrix2d_determinant(m2));
    
    // 9. Обратная матрица (для 2x2)
    printf("\n9. Обратная матрица (создадим матрицу 2x2):\n");
    matrix2d* m5 = matrix2d_create(2, true);
    matrix2d_set_element(m5, 0, 0, 4);
    matrix2d_set_element(m5, 0, 1, 3);
    matrix2d_set_element(m5, 1, 0, 3);
    matrix2d_set_element(m5, 1, 1, 2);
    matrix2d_print(m5);
    
    printf("Определитель = %d\n", matrix2d_determinant(m5));
    matrix2d* m6 = matrix2d_inverse(m5);
    if (m6 != NULL) {
        printf("Обратная матрица:\n");
        matrix2d_print(m6);
        matrix2d_free(m6);
    }
    
    // 10. Ввод с клавиатуры
    printf("\n10. Ввод матрицы с клавиатуры:\n");
    matrix2d* m7 = matrix2d_create(2, true);
    matrix2d_input(m7);
    matrix2d_print(m7);
    
    // 11. Освобождение памяти
    printf("\n11. Освобождение памяти:\n");
    matrix2d_free(m1);
    matrix2d_free(m2);
    matrix2d_free(m3);
    matrix2d_free(m4);
    matrix2d_free(m5);
    matrix2d_free(m7);
    
    return 0;
}
