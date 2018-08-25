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
    char buf[20];
    shmid = shmget(MEM_KEY, 100, 0777); 
    if (shmid == -1) {
        perror("shmget"); 
        exit(1);
    }
    sh_mem = shmat(shmid, NULL, 0);
    if (sh_mem == (void *)-1) {
        perror("shmat"); 
        exit(1);
    }
    printf("%s\n", sh_mem); 
    shmdt(sh_mem);
    return 0;
}
