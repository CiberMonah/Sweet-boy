#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>

const int N = 4;         
const int PLATES = 10;      

int* plates_on_table;

sem_t* mutex;               
sem_t* empty;           
sem_t* full;                

void producer() {
    for (int i = 0; i < PLATES; i++) {
        printf("Process 1: washing plate %d\n", i + 1);
        sleep(1);
        
        sem_wait(empty);
        sem_wait(mutex);

        (*plates_on_table)++;
        printf("Process 1: put plate on table; plates on table: %d\n", *plates_on_table);

        sem_post(mutex);
        sem_post(full);
    }
}

void consumer() {
    for (int i = 0; i < PLATES; i++) {
        sem_wait(full);
        sem_wait(mutex);

        (*plates_on_table)--;
        printf("Process 2: took plate from table; plates on table: %d\n", *plates_on_table);

        sem_post(mutex);
        sem_post(empty);

        printf("Process 2: dry plate; number - %d\n", i + 1);
        sleep(3);
    }
}

int main() {
    int shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    plates_on_table = shmat(shmid, NULL, 0);

    mutex = sem_open("/mutex", O_CREAT, 0644, 1);
    empty = sem_open("/empty", O_CREAT, 0644, N);
    full = sem_open("/full", O_CREAT, 0644, 0);

    pid_t pid = fork();

    if (pid == 0) {
        producer();
    } else {
        consumer();
        wait(NULL);

        sem_unlink("/mutex");
        sem_unlink("/empty");
        sem_unlink("/full");
        shmdt(plates_on_table);
        shmctl(shmid, IPC_RMID, NULL);

        printf("All work is done!\n");
    }

    return 0;
}