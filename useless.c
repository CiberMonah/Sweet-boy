#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    for (int i = 1; i < argc; i += 2) {
        pid_t pid = fork();
        
        if (pid == 0) {
            char *exe = argv[i];
            int delay = atoi(argv[i + 1]);
            
            fprintf(stdout, "program - %s will sleep - %d\n", exe, delay);
            sleep(delay);
            execl(exe, exe, NULL);
            
            exit(0);
        } else if (pid < 0) {
            perror("Fork failed\n");
            return 1;
        }
    }

    for (int i = 1; i < argc; i += 2) {
        wait(NULL);
    }

    return 0;
}
