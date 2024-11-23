#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

int file_changed(const char *src, const char *dest) {
    struct stat src_stat, dest_stat;

    if (stat(src, &src_stat) != 0) {
        perror("Error accessing source file");
        return 1;
    }

    if (stat(dest, &dest_stat) != 0) {
        return 1;
    }

    return src_stat.st_mtime > dest_stat.st_mtime;
}

void backup_directory(const char *src_dir, const char *dest_dir) {
    DIR *dir = opendir(src_dir);
    struct dirent *entry;

    if (dir == NULL) {
        perror("Error opening source directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        char src_path[4096];
        char dest_path[4096];
        snprintf(src_path, sizeof(src_path), "%s/%s", src_dir, entry->d_name);
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest_dir, entry->d_name);

        struct stat entry_stat;
        if (stat(src_path, &entry_stat) != 0) {
            perror("Error accessing file/directory");
            continue;
        }

        if (S_ISDIR(entry_stat.st_mode)) {
            struct stat dest_entry_stat;
            if (stat(dest_path, &dest_entry_stat) != 0) {
                char mkdir_cmd[4096];
                snprintf(mkdir_cmd, sizeof(mkdir_cmd), "mkdir -p \"%s\"", dest_path);
                if (system(mkdir_cmd) != 0) {
                    fprintf(stderr, "Failed to create directory: %s\n", dest_path);
                    continue;
                } else {
                    printf("Created directory: %s\n", dest_path);
                }
            }
            backup_directory(src_path, dest_path);
        }
        else if (S_ISREG(entry_stat.st_mode)) {
            if (file_changed(src_path, dest_path)) {
                char cp_cmd[8192];
                snprintf(cp_cmd, sizeof(cp_cmd), "cp \"%s\" \"%s\"", src_path, dest_path);
                if (system(cp_cmd) != 0) {
                    fprintf(stderr, "Failed to copy file: %s to %s\n", src_path, dest_path);
                } else {
                    printf("Copied/Updated file: %s -> %s\n", src_path, dest_path);
                }
            }
        }
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_directory> <destination_directory>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *src_directory = argv[1];
    const char *dest_directory = argv[2];

    struct stat dest_stat;
    if (stat(dest_directory, &dest_stat) != 0) {
        char mkdir_cmd[4096];
        snprintf(mkdir_cmd, sizeof(mkdir_cmd), "mkdir -p \"%s\"", dest_directory);
        if (system(mkdir_cmd) != 0) {
            fprintf(stderr, "Failed to create destination directory: %s\n", dest_directory);
            return EXIT_FAILURE;
        } else {
            printf("Created destination directory: %s\n", dest_directory);
        }
    } else if (!S_ISDIR(dest_stat.st_mode)) {
        fprintf(stderr, "Destination exists but is not a directory.\n");
        return EXIT_FAILURE;
    }

    backup_directory(src_directory, dest_directory);

    return EXIT_SUCCESS;
}
