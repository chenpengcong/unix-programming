#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGC 10
char* new_str(char *str);
int main()
{
    char *args[MAX_ARGC];
    char cmd[20];
    char buf[20];
    char index = 0;
    int i = 0;
    pid_t pid;
    while (1) {
        index = 0;
        printf("$ ");
        if (fgets(buf, 20, stdin) && buf[0] != '\n') {
            args[index++] = new_str(buf);
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
