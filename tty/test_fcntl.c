#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char *str = "hello fcntl";
    int tmpfd = open("tmp_data", O_CREAT | O_WRONLY, 0644);
    if (tmpfd == -1) {
        perror("tmp_data"); 
        exit(1);
    }
    int state_flag = fcntl(tmpfd, F_GETFL);
    if (state_flag == -1) {
        perror("get status flags failed"); 
    }
    state_flag |= O_SYNC;
    
    if (fcntl(tmpfd, F_SETFL, state_flag) == -1) {
        perror("set O_SYNC failed"); 
        exit(1);
    }
    write(tmpfd, str, strlen(str));
    close(tmpfd);
    return 0;
}
