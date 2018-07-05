#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
    int src_fd;
    int dst_fd;
    int size;
    char buf[BUFFER_SIZE];
    if (argc != 3) {
        fprintf(stderr, "missing file operand\n"); 
        exit(1);
    }
    if (strcmp(argv[1], argv[2]) == 0) {
        fprintf(stderr, "\'%s\' and \'%s\' are the same file\n", argv[1], argv[2]);
        exit(1);
    }
    src_fd = open(argv[1], O_RDONLY);
    if (src_fd == -1) {
        perror(argv[1]); 
        exit(1);
    }
    umask(0000);
    dst_fd = creat(argv[2], 0644);//man creat: the mode of the created file is (mode&~umask) 
    if (dst_fd == -1) {
        perror(argv[2]); 
        exit(1);
    }
    while ((size = read(src_fd, buf, BUFFER_SIZE)) > 0) {
        if (write(dst_fd, buf, size) < size) {
            fprintf(stderr, "can not write %s", argv[2]);
            exit(1); 
        }
    }
    if (size == -1) {
        perror(argv[1]); 
        exit(1);
    }
    if ((close(src_fd) == -1) || (close(dst_fd) == -1)) {
        perror("close file"); 
        exit(1);
    }
    return 0;
}
