#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Подключаем наши заголовочные файлы строго по названиям из заданий
#include "bitrgbled.h"
#include "matrix2d.h"
#include "contqueue.h"
#include "fiostruct.h"

int main(int argc, char *argv[]) {
    // Если программу запустили без параметров, выводим небольшую справку
    if (argc < 2) {
        printf("--- ГЛАВНОЕ МЕНЮ ТЕСТИРОВАНИЯ ---\n");
        printf("Введите один из параметров при запуске:\n");
        printf(" ./main test   - запустить тесты для лаб 1, 2 и 3 в памяти\n");
        printf(" ./main save <имя_файла> - (Лаба 4) сгенерировать структуры и сохранить в файл\n");
        printf(" ./main list <имя_файла> - (Лаба 4) посмотреть количество элементов\n");
        printf(" ./main get <индекс> <имя_файла> - (Лаба 4) загрузить конкретную матрицу\n");
        return 0;
    }

    const char* command = argv[1];

    // =================================================================
    // ТЕСТИРОВАНИЕ ЛАБОРАТОРНЫХ 1, 2 и 3 (РАБОТА В ОПЕРАТИВНОЙ ПАМЯТИ)
    // =================================================================
    if (strcmp(command, "test") == 0) {
        printf("\n>>> ТЕСТ ЛАБОРАТОРНЫХ 1 И 2 (Матрица + Битовые поля) <<<\n");
        
        // Создаем матрицу (matrix2d), которая внутри хранит пиксели (rgbled)
        matrix2d* my_matrix = create_matrix(2, 2);
        fill_random(my_matrix);
        
        printf("Сгенерирована матрица пикселей:\n");
        print_matrix(my_matrix);
        
        // Показываем преподавателю, что битовые поля реально работают
        printf("\nПроверяем первый пиксель [0][0]:\n");
        printf("Канал RED: %d\n", my_matrix->data[0][0].red);
        printf("Режим работы (mode): %d (переменная занимает всего 2 бита!)\n", my_matrix->data[0][0].mode);

        printf("\n>>> ТЕСТ ЛАБОРАТОРНОЙ 3 (Динамическая очередь) <<<\n");
        
        // Создаем контейнер contqueue
        queue* my_queue = create_queue();
        printf("Очередь создана.\n");
        
        // Кладем нашу первую матрицу в очередь
        enqueue(my_queue, my_matrix);
        printf("Добавили матрицу 2x2. Элементов в очереди: %d\n", my_queue->count);
        
        // Создаем и кладем вторую, чтобы показать, что список работает
        matrix2d* second_matrix = create_matrix(1, 1);
        fill_random(second_matrix);
        enqueue(my_queue, second_matrix);
        printf("Добавили матрицу 1x1. Элементов в очереди: %d\n", my_queue->count);
        
        // Извлекаем элемент из очереди (принцип FIFO - первым вошел, первым вышел)
        matrix2d* extracted = dequeue(my_queue);
        printf("\nИзвлекли первый элемент из очереди. Его размер: %dx%d\n", extracted->rows, extracted->cols);
        printf("Осталось элементов в очереди: %d\n", my_queue->count);

        // Аккуратно освобождаем выделенную память
        free_matrix(extracted); // Удаляем ту, которую достали
        free_queue(my_queue);   // Удаляем очередь и всё, что в ней осталось (second_matrix)
        printf("\nТестирование 1-3 завершено. Память очищена.\n");
    } 
    
    // =================================================================
    // ТЕСТИРОВАНИЕ ЛАБОРАТОРНОЙ 4 (РАБОТА С ФАЙЛАМИ)
    // =================================================================
    else if (strcmp(command, "save") == 0 && argc == 3) {
        const char* filename = argv[2];
        int count = 100; // Для быстрого теста сохраняем 100 штук, чтобы не ждать долго
        
        printf("Генерация %d матриц...\n", count);
        matrix2d** array_of_matrices = rand_gen_struct(count);
        
        printf("Сохраняем в текстовый файл '%s'...\n", filename);
        save_text(filename, array_of_matrices, count);
        
        // Освобождаем память после сохранения
        for(int i = 0; i < count; i++) {
            free_matrix(array_of_matrices[i]);
        }
        free(array_of_matrices);
        
        printf("Успешно сохранено!\n");
    }
    else if (strcmp(command, "list") == 0 && argc == 3) {
        const char* filename = argv[2];
        list_file(filename); // Функция сама откроет файл, прочитает количество и выведет в консоль
    }
    else if (strcmp(command, "get") == 0 && argc == 4) {
        int target_index = atoi(argv[2]); // Функция atoi переводит строку в число
        const char* filename = argv[3];
        
        printf("Ищем матрицу под индексом %d в файле...\n", target_index);
        
        // Читаем файл только до нужного момента
        matrix2d* found_matrix = get_element_from_text_file(filename, target_index);
        
        if (found_matrix != NULL) {
            printf("Матрица успешно найдена и загружена в память!\n");
            print_matrix(found_matrix);
            free_matrix(found_matrix); // Поработали - убрали за собой
        } else {
            printf("Ошибка: Матрица с индексом %d не найдена.\n", target_index);
        }
    }
    else {
        printf("Неверная команда или количество аргументов! Запустите программу без параметров для вызова справки.\n");
    }

    return 0; // Корректное завершение программы
}
