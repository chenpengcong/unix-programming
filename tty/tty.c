#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 512
int main(int argc, char *argv[])
{
    int tty_fd;
    char buf[BUFFER_SIZE];
    tty_fd = open(argv[1], O_WRONLY); 
    if (tty_fd < 0) {
        fprintf(stderr, "open %s failed\n", argv[1]); 
        exit(1);
    }
    while (fgets(buf, BUFFER_SIZE, stdin)) {
        write(tty_fd, buf, strlen(buf)); 
    }
    close(tty_fd);
    return 0;
}
