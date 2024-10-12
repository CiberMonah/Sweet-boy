// message.h
#ifndef MESSAGE_H
#define MESSAGE_H

#include <sys/types.h>

#define SERVER_QUEUE_NAME "/server_queue"
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGE_SIZE 256

typedef struct {
    pid_t pid;
    char message_text[MAX_MESSAGE_SIZE]; 
} message_t;

#endif