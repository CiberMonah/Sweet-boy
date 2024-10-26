//Andrew_G version

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

// Структура для передачи данных в поток
typedef struct {
    int thread_id;      // Идентификатор потока
    int num_threads;    // Общее количество потоков
    int m;              // Количество строк в матрице A
    int n;              // Количество столбцов в матрице A (и строк в B)
    int p;              // Количество столбцов в матрице B
    int **A;            // Матрица A
    int **B;            // Матрица B
    int **C;            // Результирующая матрица C
} thread_data_t;

void* multiply_matrices_threaded(void* arg) {
    thread_data_t *data = (thread_data_t*) arg;
    int thread_id = data->thread_id;
    int num_threads = data->num_threads;
    int m = data->m;
    int n = data->n;
    int p = data->p;
    int **A = data->A;
    int **B = data->B;
    int **C = data->C;

    int cols_per_thread = p / num_threads;
    int remainder = p % num_threads;
    int start_col, end_col;

    if (thread_id < remainder) {
        start_col = thread_id * (cols_per_thread + 1);
        end_col = start_col + cols_per_thread + 1;
    } else {
        start_col = thread_id * cols_per_thread + remainder;
        end_col = start_col + cols_per_thread;
    }

    for (int i = 0; i < m; i++) {
        for (int j = start_col; j < end_col; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

void multiply_matrices_single(int **A, int **B, int **C, int m, int n, int p) {
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            C[i][j] = 0;
            for (int k = 0; k < n; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

double get_time_diff(struct timespec start, struct timespec end) {
    double start_sec = start.tv_sec + start.tv_nsec / 1e9;
    double end_sec = end.tv_sec + end.tv_nsec / 1e9;
    return end_sec - start_sec;
}

int main() {
    int m, n, p;
    int num_threads;

    // Ввод размеров матриц
    printf("Введите количество строк матрицы A (m): ");
    if (scanf("%d", &m) != 1 || m <= 0) {
        fprintf(stderr, "Неверный ввод для m.\n");
        return EXIT_FAILURE;
    }

    printf("Введите количество столбцов матрицы A и строк матрицы B (n): ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        fprintf(stderr, "Неверный ввод для n.\n");
        return EXIT_FAILURE;
    }

    printf("Введите количество столбцов матрицы B (p): ");
    if (scanf("%d", &p) != 1 || p <= 0) {
        fprintf(stderr, "Неверный ввод для p.\n");
        return EXIT_FAILURE;
    }

    printf("Введите элементы матрицы A (%d x %d):\n", m, n);
    int **A = (int**) malloc(m * sizeof(int*));
    if (A == NULL) {
        fprintf(stderr, "Ошибка выделения памяти для матрицы A.\n");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < m; i++) {
        A[i] = (int*) malloc(n * sizeof(int));
        if (A[i] == NULL) {

            fprintf(stderr, "Ошибка выделения памяти для матрицы A[%d].\n", i);
            return EXIT_FAILURE;
        }
        for (int j = 0; j < n; j++) {
            if (scanf("%d", &A[i][j]) != 1) {
                fprintf(stderr, "Неверный ввод для A[%d][%d].\n", i, j);
                return EXIT_FAILURE;
            }
        }
    }

    printf("Введите элементы матрицы B (%d x %d):\n", n, p);
    int **B = (int**) malloc(n * sizeof(int*));
    if (B == NULL) {
        fprintf(stderr, "Ошибка выделения памяти для матрицы B.\n");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < n; i++) {
        B[i] = (int*) malloc(p * sizeof(int));
        if (B[i] == NULL) {
            fprintf(stderr, "Ошибка выделения памяти для матрицы B[%d].\n", i);
            return EXIT_FAILURE;
        }
        for (int j = 0; j < p; j++) {
            if (scanf("%d", &B[i][j]) != 1) {
                fprintf(stderr, "Неверный ввод для B[%d][%d].\n", i, j);
                return EXIT_FAILURE;
            }
        }
    }

    printf("Введите количество потоков: ");
    if (scanf("%d", &num_threads) != 1 || num_threads <= 0) {
        fprintf(stderr, "Неверный ввод для количества потоков.\n");
        return EXIT_FAILURE;
    }
    if (num_threads > p) {
        num_threads = p;
        printf("Количество потоков установлено в %d (количество столбцов).\n", num_threads);
    }

    int **C_single = (int**) malloc(m * sizeof(int*));
    if (C_single == NULL) {
        fprintf(stderr, "Ошибка выделения памяти для матрицы C_single.\n");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < m; i++) {
        C_single[i] = (int*) malloc(p * sizeof(int));
        if (C_single[i] == NULL) {
            fprintf(stderr, "Ошибка выделения памяти для матрицы C_single[%d].\n", i);
            return EXIT_FAILURE;
        }
    }

    int **C_threaded = (int**) malloc(m * sizeof(int*));
    if (C_threaded == NULL) {
        fprintf(stderr, "Ошибка выделения памяти для матрицы C_threaded.\n");
        return EXIT_FAILURE;
    }
    for (int i = 0; i < m; i++) {
        C_threaded[i] = (int*) malloc(p * sizeof(int));
        if (C_threaded[i] == NULL) {
            fprintf(stderr, "Ошибка выделения памяти для матрицы C_threaded[%d].\n", i);
            return EXIT_FAILURE;
        }
    }

    struct timespec start_single, end_single;
    clock_gettime(CLOCK_MONOTONIC, &start_single);
    multiply_matrices_single(A, B, C_single, m, n, p);
    clock_gettime(CLOCK_MONOTONIC, &end_single);
    double time_single = get_time_diff(start_single, end_single);

    pthread_t *threads = (pthread_t*) malloc(num_threads * sizeof(pthread_t));
    if (threads == NULL) {
        fprintf(stderr, "Ошибка выделения памяти для потоков.\n");
        return EXIT_FAILURE;
    }

    thread_data_t *thread_data = (thread_data_t*) malloc(num_threads * sizeof(thread_data_t));
    if (thread_data == NULL) {
        fprintf(stderr, "Ошибка выделения памяти для данных потоков.\n");
        return EXIT_FAILURE;
    }

    // Измерение времени для многопоточного умножения
    struct timespec start_threaded, end_threaded;
    clock_gettime(CLOCK_MONOTONIC, &start_threaded);

    for (int t = 0; t < num_threads; t++) {
        thread_data[t].thread_id = t;
        thread_data[t].num_threads = num_threads;
        thread_data[t].m = m;
        thread_data[t].n = n;
        thread_data[t].p = p;
        thread_data[t].A = A;
        thread_data[t].B = B;
        thread_data[t].C = C_threaded;

        int rc = pthread_create(&threads[t], NULL, multiply_matrices_threaded, (void*) &thread_data[t]);
        if (rc) {
            fprintf(stderr, "Ошибка создания потока %d, код ошибки %d\n", t, rc);
            return EXIT_FAILURE;
        }
    }

    // Ожидание завершения всех потоков
    for (int t = 0; t < num_threads; t++) {

        pthread_join(threads[t], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end_threaded);
    double time_threaded = get_time_diff(start_threaded, end_threaded);

    // Вывод результатов
    printf("\nВремя выполнения однопоточного умножения: %.6f секунд\n", time_single);
    printf("Время выполнения многопоточного умножения с использованием %d потоков: %.6f секунд\n", num_threads, time_threaded);

    // Опционально: Проверка корректности результатов
    int correct = 1;
    for (int i = 0; i < m && correct; i++) {
        for (int j = 0; j < p && correct; j++) {
            if (C_single[i][j] != C_threaded[i][j]) {
                correct = 0;
            }
        }
    }

    if (correct) {
        printf("Результаты умножения матриц совпадают.\n");
    } else {
        printf("Результаты умножения матриц НЕ совпадают!\n");
    }

    for (int i = 0; i < m; i++) free(A[i]);
    free(A);

    for (int i = 0; i < n; i++) free(B[i]);
    free(B);

    for (int i = 0; i < m; i++) free(C_single[i]);
    free(C_single);

    for (int i = 0; i < m; i++) free(C_threaded[i]);
    free(C_threaded);

    free(threads);
    free(thread_data);

    return EXIT_SUCCESS;
}
