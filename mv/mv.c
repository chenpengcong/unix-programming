#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 1024

void do_mv();
int is_directory(char *file);
void copy_and_unlink(char *src, char *dst);

int main(int argc, char *argv[])
{
    do_mv(argv[1], argv[2]);
    return 0;
}

void do_mv(char *src_file, char *dst)
{
    struct stat statbuf;
    char newpath[255];
    char *src_name;
    snprintf(newpath, 255, "%s", dst);
    if (stat(src_file, &statbuf) == -1) {
        fprintf(stderr, "can not stat ");
        perror(src_file);
        exit(1); 
    }
    if (!S_ISREG(statbuf.st_mode)) {
        fprintf(stderr, "src must be a regular file\n");   
        exit(1);
    }
    if (is_directory(dst)) {
        src_name = strrchr(src_file, '/');
        if (src_name == NULL) {
            src_name = src_file; 
        } else {
            src_name++; 
        }
        snprintf(newpath, 255, "%s/%s", dst, src_name);
    }
    if (rename(src_file, newpath) == -1) {
        if (errno == EXDEV) {
            copy_and_unlink(src_file, newpath);
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

void copy_and_unlink(char *src, char *dst)
{
    FILE *src_file;  
    FILE *dst_file;  
    char buf[BUFFER_SIZE];
    int size;
    if ((src_file = fopen(src, "r")) == NULL) {
        fprintf(stderr, "can not fopen ");
        perror(src); 
        exit(1);
    }
    if ((dst_file = fopen(dst, "w")) == NULL) {
        fprintf(stderr, "can not fopen ");
        perror(dst); 
        exit(1);
    }
    while ((size = fread(buf, 1, BUFFER_SIZE, src_file)) != 0) {
        fwrite(buf, 1, size, dst_file); 
    } 
    fclose(src_file);
    fclose(dst_file);
    unlink(src);
}
