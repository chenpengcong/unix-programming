#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define FIFO_NAME "my_fifo"
int main()
{
    int fd;
    char buf[100];
    memset(buf, 0, 100);
    if ((mkfifo(FIFO_NAME, 0644) == -1 && errno != EEXIST)) {
        perror("mkfifo"); 
        exit(1);
    }
    fd = open(FIFO_NAME, O_RDONLY);
    if (fd == -1) {
        perror("open"); 
        exit(1);
    }
    read(fd, buf, 10);
    printf("read from fifo:%s\n", buf);
    close(fd);
    return 0;
}
