#include "common.h"

int main() {
    key_t key;
    int msgid;
    struct message msg;

    // Создаём файл для ftok, если его нет (на случай, если Process A ещё не создал его)
    create_ftok_file();

    // Генерация уникального ключа с помощью ftok
    key = ftok(FTOK_PATH, PROJECT_ID);
    if (key == -1) {
        perror("ftok");
        exit(EXIT_FAILURE);
    }

    // Подключение к существующей очереди сообщений
    // Если очередь ещё не создана, ждать её создания
    while((msgid = msgget(key, 0666)) == -1) {
        if (errno == ENOENT) {
            printf("[Process B] Ожидание создания очереди сообщений...\n");
            sleep(1);
        } else {
            perror("msgget");
            exit(EXIT_FAILURE);
        }
    }

    printf("[Process B] Подключено к очереди сообщений с ID %d.\n", msgid);

    // Отправка сообщений с mtype=2
    for(int i = 1; i <= NUM_MESSAGES; i++) {
        msg.mtype = 2; // Тип сообщения
        snprintf(msg.mtext, MAX_TEXT, "Сообщение %d от Process B", i);
        if (msgsnd(msgid, &msg, strlen(msg.mtext)+1, 0) == -1) {
            perror("msgsnd в Process B");
            exit(EXIT_FAILURE);
        }
        printf("[Process B] Отправлено: %s\n", msg.mtext);
        sleep(1); // Задержка для наглядности
    }

    // Получение сообщений с mtype=1
    for(int i = 1; i <= NUM_MESSAGES; i++) {
        if (msgrcv(msgid, &msg, sizeof(msg.mtext), 1, 0) == -1) { // mtype = 1
            perror("msgrcv в Process B");
            exit(EXIT_FAILURE);
        }
        printf("[Process B] Получено: %s\n", msg.mtext);
    }

    printf("[Process B] Завершение работы.\n");

    return 0;
}