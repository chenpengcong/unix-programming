#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define FIFO_NAME "my_fifo"
int main()
{
    int fd;
    if ((mkfifo(FIFO_NAME, 0644) == -1) && (errno != EEXIST)) {
        perror("mkfifo"); 
        exit(1);
    }
    fd = open(FIFO_NAME, O_WRONLY);
    if (fd == -1) {
        perror("open"); 
        exit(1);
    }
    write(fd, "hi, fifo", 8);
    close(fd);
    return 0;
}
