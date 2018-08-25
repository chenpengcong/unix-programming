#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define CHAR_NUM 1024
#define USE_LOCK 1

void lock(int fd, int type);
void unlock(int fd);
int main()
{
    int i;
    char buf[CHAR_NUM + 1];
    int fd = open("tmp", O_RDONLY);
    buf[CHAR_NUM] = '\0';
    
    while (1) {
#if defined(USE_LOCK) && (USE_LOCK == 1)
        lock(fd, F_RDLCK);
#endif
        lseek(fd, 0, SEEK_SET);
        read(fd, buf, CHAR_NUM);
#if defined(USE_LOCK) && (USE_LOCK == 1)
        unlock(fd);
#endif
        //如果没使用锁，会遇到两种不同字符同时出现的情况
        for (i = 1;i < CHAR_NUM;i++) {
            if (buf[i] != buf[0]) {
                printf("%s\n", buf); 
                break; 
            } 
        } 
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

