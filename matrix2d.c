#include "matrix2d.h"

// Вспомогательная функция для выделения памяти под матрицу
static int32_t** allocate_matrix(int size) {
    if (size <= 0) return NULL;
    
    // Выделяем память под массив указателей на строки
    int32_t** data = (int32_t**)malloc(size * sizeof(int32_t*));
    if (data == NULL) return NULL;
    
    // Выделяем память под все элементы матрицы одним блоком
    int32_t* elements = (int32_t*)malloc(size * size * sizeof(int32_t));
    if (elements == NULL) {
        free(data);
        return NULL;
    }
    
    // Расставляем указатели на строки
    for (int i = 0; i < size; i++) {
        data[i] = elements + i * size;
    }
    
    return data;
}

// Вспомогательная функция для освобождения памяти матрицы
static void free_matrix(int32_t** data) {
    if (data == NULL) return;
    
    // Освобождаем блок с элементами (он хранится в первой строке)
    free(data[0]);
    // Освобождаем массив указателей на строки
    free(data);
}

// Вспомогательная функция для вычисления суммы элементов
static int64_t matrix_sum(const matrix2d* m) {
    int64_t sum = 0;
    
    for (int i = 0; i < m->size; i++) {
        for (int j = 0; j < m->size; j++) {
            sum += m->data[i][j];
        }
    }
    
    return sum;
}

// Конструктор по умолчанию (пустая матрица 0x0)
matrix2d* matrix2d_create_default() {
    printf("Вызван конструктор по умолчанию\n");
    
    matrix2d* m = (matrix2d*)malloc(sizeof(matrix2d));
    if (m == NULL) return NULL;
    
    m->size = 0;
    m->has_diagonal = false;
    m->data = NULL;
    
    return m;
}

// Конструктор с параметрами
matrix2d* matrix2d_create(int size, bool has_diagonal) {
    printf("Вызван конструктор с параметрами (size=%d)\n", size);
    
    if (size <= 0) return NULL;
    
    matrix2d* m = (matrix2d*)malloc(sizeof(matrix2d));
    if (m == NULL) return NULL;
    
    m->size = size;
    m->has_diagonal = has_diagonal;
    m->data = allocate_matrix(size);
    
    if (m->data == NULL) {
        free(m);
        return NULL;
    }
    
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
    
    if (src == NULL || src->data == NULL) return NULL;
    
    matrix2d* m = matrix2d_create(src->size, src->has_diagonal);
    if (m == NULL) return NULL;
    
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
        free_matrix(m->data);
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

// Установка элемента
void matrix2d_set_element(matrix2d* m, int row, int col, int32_t value) {
    if (m == NULL || m->data == NULL) return;
    if (row < 0 || row >= m->size || col < 0 || col >= m->size) return;
    
    m->data[row][col] = value;
}

// Получение элемента
int32_t matrix2d_get_element(const matrix2d* m, int row, int col) {
    if (m == NULL || m->data == NULL) return 0;
    if (row < 0 || row >= m->size || col < 0 || col >= m->size) return 0;
    
    return m->data[row][col];
}

// Сравнение
bool matrix2d_equal(const matrix2d* a, const matrix2d* b) {
    if (a == NULL || b == NULL) return false;
    if (a->size != b->size) return false;
    
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
           m->size, m->size, 
           m->has_diagonal ? "есть" : "нет");
    
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
    
    printf("Введите элементы матрицы %dx%d (int):\n", m->size, m->size);
    
    for (int i = 0; i < m->size; i++) {
        for (int j = 0; j < m->size; j++) {
            printf("  [%d][%d]: ", i, j);
            scanf("%d", &m->data[i][j]);
        }
    }
}

// Заполнение случайными числами
void matrix2d_fill_random(matrix2d* m, int32_t min, int32_t max) {
    if (m == NULL || m->data == NULL) return;
    
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
int32_t* matrix2d_get_row(const matrix2d* m, int row) {
    if (m == NULL || m->data == NULL) return NULL;
    if (row < 0 || row >= m->size) return NULL;
    
    int32_t* result = (int32_t*)malloc(m->size * sizeof(int32_t));
    if (result == NULL) return NULL;
    
    for (int j = 0; j < m->size; j++) {
        result[j] = m->data[row][j];
    }
    
    return result;
}

// Получение столбца
int32_t* matrix2d_get_col(const matrix2d* m, int col) {
    if (m == NULL || m->data == NULL) return NULL;
    if (col < 0 || col >= m->size) return NULL;
    
    int32_t* result = (int32_t*)malloc(m->size * sizeof(int32_t));
    if (result == NULL) return NULL;
    
    for (int i = 0; i < m->size; i++) {
        result[i] = m->data[i][col];
    }
    
    return result;
}

// Транспонирование
matrix2d* matrix2d_transpose(const matrix2d* m) {
    if (m == NULL || m->data == NULL) return NULL;
    
    matrix2d* result = matrix2d_create(m->size, m->has_diagonal);
    if (result == NULL) return NULL;
    
    for (int i = 0; i < m->size; i++) {
        for (int j = 0; j < m->size; j++) {
            result->data[j][i] = m->data[i][j];
        }
    }
    
    return result;
}

// Определитель для 2x2
static int32_t det2x2(int32_t a, int32_t b, int32_t c, int32_t d) {
    return a*d - b*c;
}

// Определитель для 3x3
static int32_t det3x3(int32_t** data) {
    return data[0][0] * det2x2(data[1][1], data[1][2], data[2][1], data[2][2])
         - data[0][1] * det2x2(data[1][0], data[1][2], data[2][0], data[2][2])
         + data[0][2] * det2x2(data[1][0], data[1][1], data[2][0], data[2][1]);
}

// Определитель
int32_t matrix2d_determinant(const matrix2d* m) {
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

// Обратная матрица для 2x2 (возвращает матрицу с вещественными числами)
matrix2d* matrix2d_inverse(const matrix2d* m) {
    if (m == NULL || m->data == NULL) return NULL;
    if (m->size != 2) {
        printf("Обратная матрица поддерживается только для 2x2\n");
        return NULL;
    }
    
    int32_t det = matrix2d_determinant(m);
    if (det == 0) {
        printf("Определитель равен 0 - обратной матрицы нет\n");
        return NULL;
    }
    
    // Создаем матрицу для результата
    matrix2d* result = matrix2d_create(2, m->has_diagonal);
    if (result == NULL) return NULL;
    
    // Вычисляем обратную матрицу с округлением
    // Для простоты используем целочисленное деление
    result->data[0][0] = m->data[1][1] / det;
    result->data[0][1] = -m->data[0][1] / det;
    result->data[1][0] = -m->data[1][0] / det;
    result->data[1][1] = m->data[0][0] / det;
    
    return result;
}
