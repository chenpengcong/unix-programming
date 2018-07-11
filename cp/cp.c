#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <string.h>

#define BUFFER_SIZE 1024

void copy_file(char *src_file, char *dst_file);
void do_copy(char *src_file, char *dst_file);
int is_directory(char *file);
void copy_dir(char *src_dir, char *dst_dir);
int main(int argc, char *argv[])
{
    char dst_file_path[NAME_MAX];
    char *src_file_name;
    if (argc < 3) {
        fprintf(stderr, "missing file operand\n"); 
        exit(1);
    }
    if (strcmp(argv[1], argv[2]) == 0) {
        fprintf(stderr, "\'%s\' and \'%s\' are the same file\n", argv[1], argv[2]);
        exit(1);
    }

    if (is_directory(argv[1])) {
        if (is_directory(argv[2])) {
            copy_dir(argv[1], argv[2]); 
        } else {
            fprintf(stderr, "%s is not a directory\n", argv[2]); 
        } 
    } else {
        if (is_directory(argv[2])) {
            /*
             * 截取源文件的文件名
             */
            src_file_name = strrchr(argv[1], '/');
            if (src_file_name == NULL) {
                src_file_name = argv[1]; 
            } else {
                src_file_name++; 
            }
            snprintf(dst_file_path, NAME_MAX, "%s/%s", argv[2], src_file_name);
            copy_file(argv[1], dst_file_path);
        } else {
            copy_file(argv[1], argv[2]);
        } 
    }
    return 0;
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

void copy_dir(char *src_dir, char *dst_dir)
{
    struct dirent *dir_entry = NULL;
    DIR *dir = opendir(src_dir);
    char dst_file_path[NAME_MAX];
    char src_file_path[NAME_MAX];
    if (dir == NULL) {
        perror(src_dir); 
        exit(1);
    }
    while ((dir_entry = readdir(dir)) != NULL) {
        snprintf(src_file_path, NAME_MAX, "%s/%s", src_dir, dir_entry->d_name); 
        snprintf(dst_file_path, NAME_MAX, "%s/%s", dst_dir, dir_entry->d_name); 
        //skip cp directory
        if (is_directory(src_file_path)) {
            printf("skip %s\n", src_file_path);
            continue; 
        }
        copy_file(src_file_path, dst_file_path);
    }
    closedir(dir);
}

void copy_file(char *src_file, char *dst_file)
{
    int src_fd;
    int dst_fd;
    int size;
    char buf[BUFFER_SIZE];
    src_fd = open(src_file, O_RDONLY);
    if (src_fd == -1) {
        perror(src_file); 
        exit(1);
    }
    umask(0000);
    dst_fd = creat(dst_file, 0644);//man creat: the mode of the created file is (mode&~umask) 
    if (dst_fd == -1) {
        perror(dst_file); 
        exit(1);
    }
    while ((size = read(src_fd, buf, BUFFER_SIZE)) > 0) {
        if (write(dst_fd, buf, size) < size) {
            fprintf(stderr, "can not write %s", dst_file);
            exit(1); 
        }
    }
    if (size == -1) {
        perror(src_file); 
        exit(1);
    }
    if ((close(src_fd) == -1) || (close(dst_fd) == -1)) {
        perror("close file"); 
        exit(1);
    }
}
