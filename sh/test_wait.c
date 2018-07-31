#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void parent_code(int child_pid);
void child_code(int delay);

int main()
{
    pid_t pid;
    if ((pid = fork()) == -1) {
        perror("fork"); 
        exit(1);
    }

    if (pid == 0) {
        child_code(3); 
    } else {
        parent_code(pid); 
    }
    return 0;
}

void child_code(int delay) 
{
    printf("child %d here, will sleep for %d seconds\n", getpid(), delay);
    sleep(delay); 
    printf("child done.  about to exit\n");
    exit(17);
}

void parent_code(int child_pid)
{
    int wait_status;
    pid_t pid = wait(&wait_status);
    printf("WIFEXITED(wait_status):%d\n", WIFEXITED(wait_status));
    printf("WEXITSTATUS(wait_status):%d\n", WEXITSTATUS(wait_status));
    printf("wait return %d\n", pid);
}
