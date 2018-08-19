#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>


//此例子演示shell程序重定向输出到文件原理
int main()
{
    pid_t pid;
    int fd;
    if ((pid = fork()) == 0) {
        //child 
        close(1);
        fd = open("tmp", O_WRONLY | O_CREAT, 0644);
        execlp("who", "who", NULL); 
        perror("execlp");
        exit(1);
    } else if(pid != -1) {
        //parent
        wait(NULL);
        printf("Done\n");
    } else {
        perror("fork");  
        exit(1);
    }
    return 0;  
}

