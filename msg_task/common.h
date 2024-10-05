#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

// Путь к файлу для ftok
#define FTOK_PATH "/tmp/msgqueuefile"
#define PROJECT_ID 'B'

// Размер текста сообщения
#define MAX_TEXT 100
// Количество сообщений для обмена
#define NUM_MESSAGES 5

// Структура сообщения
struct message {
    long mtype;
    char mtext[MAX_TEXT];
};

// Функция для создания файла, необходимого для ftok
static inline void create_ftok_file() {
    struct stat st;
    if (stat(FTOK_PATH, &st) == -1) {
        // Файл не существует, создаём его
        FILE *fp = fopen(FTOK_PATH, "w");
        if (fp == NULL) {
            perror("fopen");
            exit(EXIT_FAILURE);
        }
        fclose(fp);
        printf("Создан файл для ftok: %s\n", FTOK_PATH);
    }
}

#endif // COMMON_H