#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CHAR_NUM 1024
#define USE_LOCK 1

void lock(int fd, int type);
void unlock(int fd);
int main()
{
    char buf[CHAR_NUM];
    char c = 'A';
    int fd = open("tmp", O_CREAT | O_WRONLY);
    memset(buf, c, CHAR_NUM);
    
        while (1) {
        c++;
        if (c > 'Z') {
            c = 'A';
        }
        if (buf[0] == '0') {
            exit(1); 
        }
#if defined(USE_LOCK) && (USE_LOCK == 1)
        lock(fd, F_WRLCK);
#endif
        lseek(fd, 0, SEEK_SET);   
        write(fd, buf, CHAR_NUM);
#if defined(USE_LOCK) && (USE_LOCK == 1)
        unlock(fd);
#endif
        memset(buf, c, CHAR_NUM);
    }
    return 0;
}

void lock(int fd, int type)
{
    struct flock lock;
    lock.l_type = type;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_pid = getpid();
    if (-1 == fcntl(fd, F_SETLKW, &lock)) {
        perror("fcntl");
        exit(1);
    }
}


void unlock(int fd)
{
    lock(fd, F_UNLCK);
}
