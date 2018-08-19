#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

//模拟shell使用管道

void pipe_core(int n, char **cmds);
int main(int argc, char *argv[])
{
    pid_t pid;
    if (argc < 2) {
        fprintf(stderr, "%s\n", "missing paremeters"); 
        exit(1);
    }
    pid = fork();
    if (pid == -1) {
        perror("fork"); 
        exit(1);
    }
    if (pid == 0) {
        //child 
        pipe_core(argc - 1, argv + 1);
    } else {
        //parent 
        wait(NULL);
    }
    return 0;
}

void pipe_core(int n, char **cmds)
{
    int pipefd[2];
    int pid;
    if (n == 1) {
        execlp(cmds[0], cmds[0], NULL);
    }
    if (pipe(pipefd) == -1) {
        perror("pipe"); 
        exit(1);
    }
    pid = fork();
    if (pid == -1) {
        perror("fork"); 
        exit(1);
    }
    if (pid == 0) {
        //child 
        if (dup2(pipefd[1], 1) == -1) {
            perror("dup2"); 
            exit(1);
        }
        if ((close(pipefd[0]) == -1) || (close(pipefd[1]) == -1)) {
            perror("close"); 
            exit(1);
        }
        pipe_core(n - 1, cmds);
    } else {
        //parent 
        if (dup2(pipefd[0], 0) == -1) {
            perror("dup2"); 
            exit(1);
        }
        if ((close(pipefd[0]) == -1) || (close(pipefd[1]) == -1)) {
            perror("close"); 
            exit(1);
        }
        execlp(cmds[n - 1], cmds[n - 1], NULL);
    }

}
