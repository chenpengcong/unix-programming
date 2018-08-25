#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

#define MEM_KEY 99

int main()
{
    int shmid = 0;
    char *sh_mem = NULL;
    char *msg = "hello, shared memory!";
    shmid = shmget(MEM_KEY, 100, IPC_CREAT | 0777); 
    if (shmid == -1) {
        perror("shmget"); 
        exit(1);
    }
    sh_mem = shmat(shmid, NULL, 0);
    if (*sh_mem == -1) {
        perror("shmat"); 
        exit(1);
    }
    memcpy(sh_mem, msg, strlen(msg));
    getchar();
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
