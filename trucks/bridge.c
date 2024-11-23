#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NORTH 1
#define SOUTH 2

typedef struct {
    int count;          // Количество машин на мосту
    int direction;      // Направление движения на мосту (1 - север→юг, 2 - юг→север, 0 - мост пуст)
    sem_t mutex;        // Мьютекс для взаимного исключения
    sem_t north_queue;  // Очередь для машин, движущихся с севера на юг
    sem_t south_queue;  // Очередь для машин, движущихся с юга на север
} BridgeMonitor;

// Инициализация моста
BridgeMonitor bridge = {0, 0, 0, 0, 0};

// Функция инициализации мостового монитора
void init_bridge(BridgeMonitor *b) {
    b->count = 0;
    b->direction = 0;
    sem_init(&b->mutex, 0, 1);        // Инициализируем мьютекс как бинарный семафор
    sem_init(&b->north_queue, 0, 0);  // Инициализируем очередь для севера
    sem_init(&b->south_queue, 0, 0);  // Инициализируем очередь для юга
}

void north_entry(BridgeMonitor *b, int truck_id) {
    sem_wait(&b->mutex);
    if ((b->count == 2) || (b->count == 1 && b->direction != NORTH)) {
        printf("Самосвал %d ожидает на севере\n", truck_id);
        sem_post(&b->mutex);
        sem_wait(&b->north_queue);
        sem_wait(&b->mutex);
    }

    b->count++;
    b->direction = NORTH;
    printf("Самосвал %d въезжает на мост с севера\n", truck_id);

    if (b->count == 1) {
        sem_post(&b->north_queue);
    }
    sem_post(&b->mutex);
}

void north_exit(BridgeMonitor *b, int truck_id) {
    sem_wait(&b->mutex);
    b->count--;
    printf("Самосвал %d выходит с моста на юге\n", truck_id);
    if (b->count == 0) {
        b->direction = 0;
        sem_post(&b->south_queue);
    } else {
        sem_post(&b->north_queue);
    }
    sem_post(&b->mutex);
}

void south_entry(BridgeMonitor *b, int truck_id) {
    sem_wait(&b->mutex);
    if ((b->count == 2) || (b->count == 1 && b->direction != SOUTH)) {
        printf("Самосвал %d ожидает на юге\n", truck_id);
        sem_post(&b->mutex);
        sem_wait(&b->south_queue);
        sem_wait(&b->mutex);
    }
    b->count++;
    b->direction = SOUTH;
    printf("Самосвал %d въезжает на мост с юга\n", truck_id);
    if (b->count == 1) {
        sem_post(&b->south_queue);
    }
    sem_post(&b->mutex);
}
void south_exit(BridgeMonitor *b, int truck_id) {
    sem_wait(&b->mutex);
    b->count--;
    printf("Самосвал %d выходит с моста на севере\n", truck_id);
    if (b->count == 0) {
        b->direction = 0;
        sem_post(&b->north_queue);
    } else {
        sem_post(&b->south_queue);
    }
    sem_post(&b->mutex);
}

typedef struct {
    int id;
    int direction;
} Truck;

void* truck_process(void* arg) {
    Truck *truck = (Truck*) arg;
    BridgeMonitor *b = &bridge;

    while (1) {
        if (truck->direction == NORTH) {
            north_entry(b, truck->id);
            printf("Самосвал %d едет по мосту с севера на юг\n", truck->id);
            sleep(rand() % 3 + 1);

            north_exit(b, truck->id);
        } else {
            south_entry(b, truck->id);
            printf("Самосвал %d едет по мосту с юга на север\n", truck->id);
            sleep(rand() % 3 + 1); 
            south_exit(b, truck->id);
        }

        printf("Самосвал %d доставляет груз и возвращается обратно\n", truck->id);
        sleep(rand() % 5 + 1);

        truck->direction = (truck->direction == NORTH) ? SOUTH : NORTH;
    }

    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));
    init_bridge(&bridge);

    int NUM_TRUCKS = 10;
    pthread_t trucks[NUM_TRUCKS];
    Truck truck_info[NUM_TRUCKS];

    for (int i = 0; i < NUM_TRUCKS; i++) {
        truck_info[i].id = i + 1;
        truck_info[i].direction = (i % 2 == 0) ? NORTH : SOUTH;
        if (pthread_create(&trucks[i], NULL, truck_process, &truck_info[i]) != 0) {
            perror("Ошибка создания потока");
            exit(1);
        }
    }

    for (int i = 0; i < NUM_TRUCKS; i++) {
        pthread_join(trucks[i], NULL);
    }
    
    sem_destroy(&bridge.mutex);
    sem_destroy(&bridge.north_queue);
    sem_destroy(&bridge.south_queue);

    return 0;
}
