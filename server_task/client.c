#include <stdio.h>
#include <stdlib.h>
#include <mqueue.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "message.h"

int main() {
    mqd_t server_queue, client_queue;
    struct mq_attr attr;
    message_t msg, reply;
    char client_queue_name[64];
    ssize_t bytes_read;

    pid_t pid = getpid();
    snprintf(client_queue_name, sizeof(client_queue_name), "/client_queue_%d", pid);

    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = sizeof(message_t);
    attr.mq_curmsgs = 0;

    client_queue = mq_open(client_queue_name, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr);
    if (client_queue == (mqd_t)-1) {
        perror("Не удалось создать клиентскую очередь");
        exit(-1);
    }

    server_queue = mq_open(SERVER_QUEUE_NAME, O_WRONLY);
    if (server_queue == (mqd_t)-1) {
        perror("Не удалось открыть серверную очередь");
        mq_close(client_queue);
        mq_unlink(client_queue_name);
        exit(-1);
    }

    printf("Введите сообщение для сервера: ");
    fgets(msg.message_text, MAX_MESSAGE_SIZE, stdin);
    msg.message_text[strcspn(msg.message_text, "\n")] = '\0';
    msg.pid = pid;

    if (mq_send(server_queue, (char*)&msg, sizeof(msg), 0) == -1) {
        perror("Не удалось отправить сообщение серверу");
        mq_close(client_queue);
        mq_unlink(client_queue_name);
        exit(-1);
    }

    mq_close(server_queue);

    //Ждем ответа от сервера
    bytes_read = mq_receive(client_queue, (char*)&reply, sizeof(reply), NULL);
    if (bytes_read >= 0) {
        printf("Ответ от сервера: %s\n", reply.message_text);
    } else {
        perror("Не удалось получить ответ от сервера");
    }

    mq_close(client_queue);
    mq_unlink(client_queue_name);

    return 0;
}