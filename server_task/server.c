#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "message.h"

int main() {
    mqd_t server_queue, client_queue;
    struct mq_attr attr;
    message_t msg;
    char client_queue_name[64];
    ssize_t bytes_read;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(message_t);
    attr.mq_curmsgs = 0;

    mq_unlink(SERVER_QUEUE_NAME);

    server_queue = mq_open(SERVER_QUEUE_NAME, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr);

    if (server_queue == (mqd_t)-1) {
        perror("Не удалось создать серверную очередь");
        exit(-1);
    }

    printf("Сервер запущен и ожидает сообщений...\n");

    while (1) {
        bytes_read = mq_receive(server_queue, (char*)&msg, sizeof(message_t), NULL);
        printf("Получено сообщение от клиента PID = %d: %s\n", msg.pid, msg.message_text);

        snprintf(client_queue_name, sizeof(client_queue_name), "/client_queue_%d", msg.pid);

        client_queue = mq_open(client_queue_name, O_WRONLY);
        if (client_queue == (mqd_t)-1) {
            perror("Не удалось открыть клиентскую очередь");
            continue;
        }

        message_t reply;
        reply.pid = getpid();

        snprintf(reply.message_text, MAX_MESSAGE_SIZE, "Сервер получил ваше сообщение: %s", msg.message_text);

        if (mq_send(client_queue, (char*)&reply, sizeof(reply), 0) == -1) {
            perror("Не удалось отправить сообщение клиенту");
        } else {
            printf("Отправлен ответ клиенту PID=%d\n", msg.pid);
        }

        mq_close(client_queue);
    }

    mq_close(server_queue);
    mq_unlink(SERVER_QUEUE_NAME);

    return 0;
}

