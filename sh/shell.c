#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_ARGC 10
char* new_str(char *str);
void int_handler(int signum);

int main()
{
    char *args[MAX_ARGC];
    struct sigaction oldact;
    char cmd[20];
    char buf[20];
    char index = 0;
    int i = 0;
    pid_t pid;
    //signal(SIGINT, int_handler);
    sigaction(SIGINT, NULL, &oldact);
    oldact.sa_flags &= ~(SA_RESTART | SA_SIGINFO);//取消SA_RESTART标志，让阻塞的fgets直接失败返回
    oldact.sa_handler = int_handler;
    sigaction(SIGINT, &oldact, NULL);
    while (1) {
        index = 0;
        printf("$ ");
        if (fgets(buf, 20, stdin) && buf[0] != '\n') {
            args[index++] = new_str(buf);
            if (strncmp(args[0], "exit", 4) == 0) {
                exit(0); 
            }
        } else {
            continue;
        }
        while (index < MAX_ARGC - 1) {
            printf("arg[%d]:", index);
            fgets(buf, 20, stdin);
            args[index] = new_str(buf);
            if (args[index][0] == '\0') {
                break; 
            }
            index++;
        }
        args[index] = NULL;
        if ((pid = fork()) == -1) {
            perror("fork");
        } else if (pid == 0) {
            execvp(args[0], args);
        } else {
            wait(NULL); 
        }

        for (i = 0;i < index;i++) {
            free(args[i]); 
        }
    }
    return 0;
}

char* new_str(char *str)
{
    char *new_s = NULL;
    char len = 0;
    len = strlen(str);
    str[len -1] = '\0';
    new_s = (char *)malloc(len);
    memcpy(new_s, str, len);
    return new_s;
}

void int_handler(int signum)
{
    printf("receive int signal\n");
}
