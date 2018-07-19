#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void do_mv();
int is_directory(char *file);
int main(int argc, char *argv[])
{
    do_mv(argv[1], argv[2]);
    return 0;
}

void do_mv(char *src_file, char *f2)
{
    struct stat statbuf;
    char newpath[255];
    char *src_name;
    if (stat(src_file, &statbuf) == -1) {
        fprintf(stderr, "can not stat ");
        perror(src_file);
        exit(1); 
    }
    if (!S_ISREG(statbuf.st_mode)) {
        fprintf(stderr, "src must be a regular file\n");   
        exit(1);
    }
    if (is_directory(f2)) {
        src_name = strrchr(src_file, '/');
        if (src_name == NULL) {
            src_name = src_file; 
        } else {
            src_name++; 
        }
        snprintf(newpath, 255, "%s/%s", f2, src_name);
        if (rename(src_file, newpath) == -1) {
            perror("rename failed");
            exit(1);
        } 
    } else {
        if (rename(src_file, f2) == -1) {
            perror("rename failed");
            exit(1);
        } 
    }

}

int is_directory(char *file)
{
    struct stat stat_buf;
    if (stat(file, &stat_buf) == -1) {
        return 0;
    }
    if (S_ISDIR(stat_buf.st_mode)) {
        return 1; 
    } else {
        return 0; 
    }
}


