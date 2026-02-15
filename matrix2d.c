#include "matrix2d.h"

// Вспомогательная функция для выделения памяти
static int** allocate_matrix(int size) {
    int** data = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        data[i] = (int*)malloc(size * sizeof(int));
    }
    return data;
}

// Вспомогательная функция для освобождения памяти
static void free_matrix(int** data, int size) {
    for (int i = 0; i < size; i++) {
        free(data[i]);
    }
    free(data);
}

// Конструктор по умолчанию (пустая матрица 0x0)
matrix2d* matrix2d_create_default() {
    printf("Вызван конструктор по умолчанию\n");
    
    matrix2d* m = (matrix2d*)malloc(sizeof(matrix2d));
    m->size = 0;
    m->has_diagonal = false;
    m->data = NULL;
    
    return m;
}

// Конструктор с параметрами
matrix2d* matrix2d_create(int size, bool has_diagonal) {
    printf("Вызван конструктор с параметрами (size=%d)\n", size);
    
    matrix2d* m = (matrix2d*)malloc(sizeof(matrix2d));
    m->size = size;
    m->has_diagonal = has_diagonal;
    m->data = allocate_matrix(size);
    
    // Заполняем нулями
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            m->data[i][j] = 0;
        }
    }
    
    return m;
}

// Конструктор копирования
matrix2d* matrix2d_copy(const matrix2d* src) {
    printf("Вызван конструктор копирования\n");
    
    if (src == NULL) return NULL;
    
    matrix2d* m = matrix2d_create(src->size, src->has_diagonal);
    
    // Копируем данные
    for (int i = 0; i < src->size; i++) {
        for (int j = 0; j < src->size; j++) {
            m->data[i][j] = src->data[i][j];
        }
    }
    
    return m;
}

// Деструктор
void matrix2d_free(matrix2d* m) {
    if (m == NULL) return;
    
    printf("Вызван деструктор для матрицы size=%d\n", m->size);
    
    if (m->data != NULL) {
        free_matrix(m->data, m->size);
    }
    free(m);
}

// Инкремент (увеличение всех элементов на 1)
void matrix2d_increment(matrix2d* m) {
    if (m == NULL || m->data == NULL) return;
    
    for (int i = 0; i < m->size; i++) {
        for (int j = 0; j < m->size; j++) {
            m->data[i][j]++;
        }
    }
}

// Декремент (уменьшение всех элементов на 1)
void matrix2d_decrement(matrix2d* m) {
    if (m == NULL || m->data == NULL) return;
    
    for (int i = 0; i < m->size; i++) {
        for (int j = 0; j < m->size; j++) {
            m->data[i][j]--;
        }
    }
}

// Установка конкретного элемента
void matrix2d_set_element(matrix2d* m, int row, int col, int value) {
    if (m == NULL || m->data == NULL) return;
    if (row < 0 || row >= m->size || col < 0 || col >= m->size) return;
    
    m->data[row][col] = value;
}

// Вычисление суммы всех элементов (для сравнения)
static int matrix_sum(const matrix2d* m) {
    int sum = 0;
    for (int i = 0; i < m->size; i++) {
        for (int j = 0; j < m->size; j++) {
            sum += m->data[i][j];
        }
    }
    return sum;
}

// Сравнение
bool matrix2d_equal(const matrix2d* a, const matrix2d* b) {
    if (a == NULL || b == NULL) return false;
    return matrix_sum(a) == matrix_sum(b);
}

bool matrix2d_not_equal(const matrix2d* a, const matrix2d* b) {
    return !matrix2d_equal(a, b);
}

bool matrix2d_greater(const matrix2d* a, const matrix2d* b) {
    if (a == NULL || b == NULL) return false;
    return matrix_sum(a) > matrix_sum(b);
}

bool matrix2d_less(const matrix2d* a, const matrix2d* b) {
    if (a == NULL || b == NULL) return false;
    return matrix_sum(a) < matrix_sum(b);
}

bool matrix2d_greater_or_equal(const matrix2d* a, const matrix2d* b) {
    if (a == NULL || b == NULL) return false;
    return matrix_sum(a) >= matrix_sum(b);
}

bool matrix2d_less_or_equal(const matrix2d* a, const matrix2d* b) {
    if (a == NULL || b == NULL) return false;
    return matrix_sum(a) <= matrix_sum(b);
}

// Вывод матрицы
void matrix2d_print(const matrix2d* m) {
    if (m == NULL) {
        printf("NULL\n");
        return;
    }
    
    printf("Матрица %dx%d (диагональ: %s):\n", 
           m->size, m->size, m->has_diagonal ? "есть" : "нет");
    
    if (m->data == NULL) {
        printf("  пустая\n");
        return;
    }
    
    for (int i = 0; i < m->size; i++) {
        printf("  ");
        for (int j = 0; j < m->size; j++) {
            printf("%4d ", m->data[i][j]);
        }
        printf("\n");
    }
}

// Ввод с клавиатуры
void matrix2d_input(matrix2d* m) {
    if (m == NULL || m->data == NULL) return;
    
    printf("Введите элементы матрицы %dx%d:\n", m->size, m->size);
    for (int i = 0; i < m->size; i++) {
        for (int j = 0; j < m->size; j++) {
            printf("  [%d][%d]: ", i, j);
            scanf("%d", &m->data[i][j]);
        }
    }
}

// Заполнение случайными числами
void matrix2d_fill_random(matrix2d* m, int min, int max) {
    if (m == NULL || m->data == NULL) return;
    
    // Инициализация генератора случайных чисел
    static int seeded = 0;
    if (!seeded) {
        srand(time(NULL));
        seeded = 1;
    }
    
    for (int i = 0; i < m->size; i++) {
        for (int j = 0; j < m->size; j++) {
            m->data[i][j] = min + rand() % (max - min + 1);
        }
    }
}

// Получение строки
int* matrix2d_get_row(const matrix2d* m, int row) {
    if (m == NULL || m->data == NULL) return NULL;
    if (row < 0 || row >= m->size) return NULL;
    
    int* result = (int*)malloc(m->size * sizeof(int));
    for (int j = 0; j < m->size; j++) {
        result[j] = m->data[row][j];
    }
    
    return result; // Нужно будет освободить free()
}

// Получение столбца
int* matrix2d_get_col(const matrix2d* m, int col) {
    if (m == NULL || m->data == NULL) return NULL;
    if (col < 0 || col >= m->size) return NULL;
    
    int* result = (int*)malloc(m->size * sizeof(int));
    for (int i = 0; i < m->size; i++) {
        result[i] = m->data[i][col];
    }
    
    return result; // Нужно будет освободить free()
}

// Транспонирование
matrix2d* matrix2d_transpose(const matrix2d* m) {
    if (m == NULL || m->data == NULL) return NULL;
    
    matrix2d* result = matrix2d_create(m->size, m->has_diagonal);
    
    for (int i = 0; i < m->size; i++) {
        for (int j = 0; j < m->size; j++) {
            result->data[j][i] = m->data[i][j];
        }
    }
    
    return result;
}

// Определитель для 2x2
static int det2x2(int a, int b, int c, int d) {
    return a*d - b*c;
}

// Определитель для 3x3
static int det3x3(int** data) {
    return data[0][0] * det2x2(data[1][1], data[1][2], data[2][1], data[2][2])
         - data[0][1] * det2x2(data[1][0], data[1][2], data[2][0], data[2][2])
         + data[0][2] * det2x2(data[1][0], data[1][1], data[2][0], data[2][1]);
}

// Определитель
int matrix2d_determinant(const matrix2d* m) {
    if (m == NULL || m->data == NULL) return 0;
    
    if (m->size == 2) {
        return det2x2(m->data[0][0], m->data[0][1], 
                      m->data[1][0], m->data[1][1]);
    }
    else if (m->size == 3) {
        return det3x3(m->data);
    }
    else {
        printf("Определитель поддерживается только для 2x2 и 3x3\n");
        return 0;
    }
}

// Обратная матрица для 2x2
matrix2d* matrix2d_inverse(const matrix2d* m) {
    if (m == NULL || m->data == NULL) return NULL;
    if (m->size != 2) {
        printf("Обратная матрица поддерживается только для 2x2\n");
        return NULL;
    }
    
    int det = matrix2d_determinant(m);
    if (det == 0) {
        printf("Определитель равен 0 - обратной матрицы нет\n");
        return NULL;
    }
    
    matrix2d* result = matrix2d_create(2, m->has_diagonal);
    
    // Формула для обратной матрицы 2x2: 1/det * [ d -b; -c a ]
    result->data[0][0] = m->data[1][1] / det;
    result->data[0][1] = -m->data[0][1] / det;
    result->data[1][0] = -m->data[1][0] / det;
    result->data[1][1] = m->data[0][0] / det;
    
    return result;
}
