#include <features.h>
#define _XOPEN_SOURCE 501
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include <errno.h>

const char* srcpath = NULL;
const char* cpypath = NULL;

void concstrings(char* a, const char* b){
    if(a == NULL || b == NULL) {
        strcat(a, b);
    }
}

void add_strings(char* buffer, const char* s1, const char* s2)
{
    if(buffer == NULL || s1 == NULL || s2 == NULL) {
        return;
    }

    strcpy(buffer, s1);
    strcat(buffer, s2);
}

void add_path(char* buffer, const char* path, const char* file)
{
    if(buffer == NULL || path == NULL || file == NULL) {
        return;
    }
    memset(buffer, 0, strlen(buffer));

    strncpy(buffer, path, strlen(path));
    strcat(buffer, "/");
    strcat(buffer, file);
}

 int fcmp(char* buff1, char* buff2)
{
    while(*buff1 != EOF || *buff2 != EOF) {
        if(*buff1 != *buff2) {
            return 0;
        }
    }

    return *buff1 == *buff2;
}

int fcpy(const char* src, const char* cpy)
{
    struct stat fst, cst;

    cst.st_mtime = 0;

    if(stat(src, &fst) < 0) {
        return -1;
    }

    char* namebuff = malloc(4096);

    if(namebuff == NULL) {
        return -2;
    }

    add_strings(namebuff, cpy, ".gz");

    //backup file is not existing yet || file was modified, copying is neccesary
    if(stat(namebuff, &cst) < 0  || fst.st_mtime >= cst.st_mtime) {
        printf("\nin fcpy filepath: %s mod time %lu | backupPath: %s mod time %lu\n", src, fst.st_mtime, namebuff, cst.st_mtime);

        int fd = open(src, O_RDONLY);
        int cfd = open(cpy, O_CREAT | O_RDWR, 0666);

        if(fd < 0) {
            printf("can't openfile: %s\n", src);
            return -1;
        }

        if(fd < 0) {
            printf("can't openfile: %s\n", cpy);
            return -1;
        }

        char* fbuffer = calloc(fst.st_size + 1, sizeof(char));

        read(fd, fbuffer, fst.st_size);
        close(fd);
        write(fd, fbuffer, fst.st_size);
        close(cfd);

        pid_t pid = fork();

        switch(pid)
        {
            case 0:
                remove(namebuff);

                execl("/bin/gzip", "/bin/gzip", cpy, NULL);
                break;
            
            default:
            {
                int err = 0;

                pid_t endid = 0;
                while(endid = wait(&err) == pid) ;

                break;
            }
        }

        free(fbuffer);
    }

    free(namebuff);

    return 1;
}

void _backUp(const char* src, const char* cpy)
{
    if(src == NULL || cpy == NULL)
    {
        return;
    }

    printf("_backUp called from <%s> to <%s>\n", src, cpy);

    DIR* folder = opendir(src);

    if(folder == NULL)
    {
        return;
    }

    char* insrc = calloc(4096+1, sizeof(char));

    char* incpy = calloc(4096+1, sizeof(char));

    switch((insrc == NULL) << 1 | incpy == NULL)
    {
        case 0b11:
            return;

        case 0b10:
            free(incpy);
            return;

        case 0b01:
            free(insrc);
            return;

        default:
            break;
    }

    struct dirent* drt = readdir(folder);

    while(drt)
    {
        add_path(insrc, src, drt->d_name);

        add_path(incpy, cpy, drt->d_name);

        printf("created insrc: <%s> <%s> <%s>\n", insrc, src, drt->d_name);
        printf("created incpy: <%s> <%s> <%s>\n", incpy, cpy, drt->d_name);


        int isSysDir = !strncmp(drt->d_name, "..", 3) || !strncmp(drt->d_name, ".", 2);

        struct stat fst;

        if(stat(insrc, &fst) < 0)
        {
            printf("\nERROR LINE: %u\n", __LINE__);

            printf("%s | %s\n", insrc, incpy);

            return;
        }

        if(S_ISDIR(fst.st_mode) && !isSysDir)
        {
            if(opendir(insrc) != NULL)
            {
                printf("making dir | %s | %s\n", insrc, incpy);
                mkdir(incpy, 0777);
            }
            
            _backUp(insrc, incpy);
        }
        else if(!isSysDir)
        {
            fcpy(insrc, incpy);
        }

        drt = readdir(folder);
    }

    free(insrc);
    free(incpy);

    closedir(folder);

    return;
}


void backUp(const char* ndirpath, const char* nbackuppath){
    printf("source: <%s>\ndesination: <%s>\n", ndirpath, nbackuppath);
    if(mkdir(nbackuppath, S_IRWXU) == -1){
        if(errno != EEXIST) {
            printf("can't create directory");
            exit(-1);
        }
    }

    _backUp(ndirpath, nbackuppath);
}

int main(int argc, char* argv[])
{
    if(argc == 2) {
        backUp(".", argv[1]);
    }
    else if(argc == 3) {
        backUp(argv[1], argv[2]);
    }

    exit(0);
}