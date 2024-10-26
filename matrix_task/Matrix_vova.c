#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define SIZE 50
#define NUM_THREADS 5

int matrix1[SIZE][SIZE];
int matrix2[SIZE][SIZE];
int result[SIZE][SIZE];
int result_seq[SIZE][SIZE];

typedef struct {
    int start_row;
    int end_row;
} ThreadData;

void initialize_matrices() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            matrix1[i][j]    = 1;
            matrix2[i][j]    = 10; 
            result[i][j]     = 0;
            result_seq[i][j] = 0;
        }
    }
}

void* multiply_part(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int start = data->start_row;
    int end = data->end_row;

    for (int i = start; i < end; i++) {
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < SIZE; k++) {
                result[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
    return NULL;
}

void multiply_sequential() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            for (int k = 0; k < SIZE; k++) {
                result_seq[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }
}

void print_matrix(int matrix[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];
    clock_t start, end;
    double time_spent;

    initialize_matrices();

    start = clock();
    multiply_sequential();
    end = clock();
    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Р’СЂРµРјСЏ РѕР±С‹С‡РЅРѕРіРѕ СѓРјРЅРѕР¶РµРЅРёСЏ: %.5f СЃРµРєСѓРЅРґ\n", time_spent);

    //-----------------------------------------

    int rows_per_thread = SIZE / NUM_THREADS;

    start = clock();
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start_row = i * rows_per_thread;
        thread_data[i].end_row = (i == NUM_THREADS - 1) ? SIZE : (i + 1) * rows_per_thread;
        pthread_create(&threads[i], NULL, multiply_part, (void*)&thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    end = clock();
    time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Р’СЂРµРјСЏ РјРЅРѕРіРѕРїРѕС‚РѕС‡РєРё: %.5f СЃРµРєСѓРЅРґ\n", time_spent);

    printf("Р РµР·СѓР»СЊС‚Р°С‚ РјРЅРѕРіРѕРїРѕС‚РѕС‡РєРё:\n");
    print_matrix(result);

    return 0;
}