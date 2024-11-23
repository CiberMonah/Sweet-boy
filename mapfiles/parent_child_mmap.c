#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/wait.h>

#define NUM_ITEMS 100000

// Структура данных для записи в файл
struct A {
    double f;
    double f2;
};

// Структура для общей памяти, включая семафор
struct SharedMemory {
    sem_t semaphore;            // Семафор для синхронизации
    struct A data[NUM_ITEMS];   // Массив данных
};

int main(void)
{
    int fd;
    size_t length;
    pid_t pid;

    // Открываем или создаем файл для маппинга
    fd = open("mapped.dat", O_RDWR | O_CREAT, 0666);
    if (fd == -1)
    {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    // Определяем длину маппинга: размер структуры SharedMemory
    length = sizeof(struct SharedMemory);

    // Устанавливаем размер файла
    if (ftruncate(fd, length) == -1)
    {
        perror("Ошибка вызова ftruncate");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Маппируем файл в память
    struct SharedMemory *shared = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared == MAP_FAILED)
    {
        perror("Ошибка маппинга файла");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // маппинг уже создан
    close(fd);

    // Инициализируем семафор в общей памяти
    if (sem_init(&shared->semaphore, 1, 0) == -1) // pshared = 1 для межпроцессного использования
    {
        perror("Ошибка инициализации семафора");
        munmap(shared, length);
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid < 0)
    {
        perror("Ошибка вызова fork");
        sem_destroy(&shared->semaphore);
        munmap(shared, length);
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
    {
        printf("Дочерний процесс: Ожидание данных от родителя...\n");
        
        // Ожидание сигнала от родителя
        if (sem_wait(&shared->semaphore) == -1)
        {
            perror("Дочерний процесс: Ошибка sem_wait");
            munmap(shared, length);
            exit(EXIT_FAILURE);
        }

        printf("Дочерний процесс: Получены данные от родителя. Чтение данных...\n");

        // Чтение данных и выполнение действий (например, вывод первых 5 записей)
        for(int i = 0; i < 5 && i < NUM_ITEMS; i++)
        {
            printf("Дочерний процесс: data[%d].f = %.2f, data[%d].f2 = %.2f\n", 
                   i, shared->data[i].f, i, shared->data[i].f2);
        }

        for(int i = 0; i < NUM_ITEMS; i++)
        {
            shared->data[i].f2 *= 1.10;
        }

        printf("Дочерний процесс: Модификация данных завершена.\n");

        munmap(shared, length);
        exit(EXIT_SUCCESS);
    }
    else
    {
        printf("Родительский процесс: Запись данных...\n");

        // Записываем данные в общую память
        for(int i = 0; i < NUM_ITEMS; i++)
        {
            shared->data[i].f = (double)i;
            shared->data[i].f2 = shared->data[i].f * shared->data[i].f;
        }

        printf("Родительский процесс: Данные записаны. Уведомление дочернего процесса...\n");

        // Отправляем сигнал дочернему процессу
        if (sem_post(&shared->semaphore) == -1)
        {
            perror("Родительский процесс: Ошибка sem_post");

            sem_destroy(&shared->semaphore);
            munmap(shared, length);
            exit(EXIT_FAILURE);
        }

        // Ожидание завершения дочернего процесса
        wait(NULL);
        printf("Родительский процесс: Дочерний процесс завершил работу.\n");

        // Уничтожаем семафор
        if (sem_destroy(&shared->semaphore) == -1)
        {
            perror("Ошибка уничтожения семафора");
        }

        // Отмэппинг памяти
        if (munmap(shared, length) == -1)
        {
            perror("Ошибка отмэппинга памяти");
            exit(EXIT_FAILURE);
        }

        printf("Родительский процесс: Завершение.\n");
        exit(EXIT_SUCCESS);
    }
}
