#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

void* producer();
void* consumer();


const int N = 4;            // How much plates on tables
const int PLATES = 10;      // All plates

sem_t mutex;                // Critical section semafore
sem_t empty;                // semofore to check if there is place
sem_t full;                 // semafore to check dirty plates

int plates_on_table = 0;    // текущее количество тарелок на столе

void* producer() {
    for (int i = 0; i < PLATES; i++) {
        printf("Process 1: washing plate %d\n", i + 1);
        sleep(1);
        
        // if table is full wait
        sem_wait(&empty);
        sem_wait(&mutex);

        // Put plate on table
        plates_on_table++;
        printf("Process 1: put plate on table; plates on table: %d\n", plates_on_table);

        sem_post(&mutex);
        sem_post(&full);
    }

    return NULL;
}

void* consumer() {
    for (int i = 0; i < PLATES; i++) {
        
        // Wait for dirty 
        sem_wait(&full);
        sem_wait(&mutex);

        // ZA RABOTU NAHUI 
        plates_on_table--;
        printf("Process 2: took plate from table; plates on table: %d\n", plates_on_table);

        sem_post(&mutex);
        sem_post(&empty);

        // Wash plate
        printf("Process 2: dry plate; number - %d\n", i + 1);
        sleep(3);
    }

    return NULL;
}

int main() {
    pthread_t prod, cons;

    // Initializing
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);

    // Create threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Wait until all process stop works
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    // delete sems
    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    printf("All work is done!\n");

    return 0;
}