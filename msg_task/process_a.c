#include "common.h"

int main() {
    key_t key;
    int msgid;
    struct message msg;

    // Создаём файл для ftok, если его нет
    create_ftok_file();

    // Генерация уникального ключа с помощью ftok
    key = ftok(FTOK_PATH, PROJECT_ID);
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Создание очереди сообщений (или подключение к существующей)
    msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("[Process A] Очередь сообщений с ID %d подключена.\n", msgid);

    // Отправка сообщений с mtype=1
    for(int i = 1; i <= NUM_MESSAGES; i++) {
        msg.mtype = 1; // Тип сообщения
        snprintf(msg.mtext, MAX_TEXT, "Сообщение %d от Process A", i);
        if (msgsnd(msgid, &msg, strlen(msg.mtext)+1, 0) == -1) {
            perror("msgsnd в Process A");
            // Попытка удалить очередь сообщений перед выходом
            msgctl(msgid, IPC_RMID, NULL);
            exit(EXIT_FAILURE);
        }
        printf("[Process A] Отправлено: %s\n", msg.mtext);
        sleep(1); // Задержка для наглядности
    }

    // Получение сообщений с mtype=2
    for(int i = 1; i <= NUM_MESSAGES; i++) {
        if (msgrcv(msgid, &msg, sizeof(msg.mtext), 2, 0) == -1) { // mtype = 2
            perror("msgrcv в Process A");
            // Попытка удалить очередь сообщений перед выходом
            msgctl(msgid, IPC_RMID, NULL);
            exit(EXIT_FAILURE);
        }
             printf("[Process A] Получено: %s\n", msg.mtext);
    }

    // Удаление очереди сообщений после использования
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl");
        exit(EXIT_FAILURE);
    }

    printf("[Process A] Очередь сообщений удалена.\n");

    return 0;
}