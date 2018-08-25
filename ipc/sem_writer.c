#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define SEM_KEY 6666

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                             (Linux-specific) */
};

void set_semaphore_val(int semid, int semnum, int val);
void wait_and_lock(int semid);
void unlock(int semid);
void clean_up();

int semid;

#define HINT1 "start writing...\n"
#define HINT2 "end writing\n"

int main()
{
    int cnt = 0;
    semid = semget(SEM_KEY, 2, IPC_CREAT| 0666);
    if (semid == -1 ) {
        perror("semget");
        exit(1); 
    }
    signal(SIGINT, clean_up);
    set_semaphore_val(semid, 0, 0); 
    set_semaphore_val(semid, 1, 0);
    while (cnt < 10) {
        wait_and_lock(semid); 
        write(1, HINT1, strlen(HINT1));//这里用printf不能及时输出到屏幕
        sleep(10);
        write(1, HINT2, strlen(HINT2));
        unlock(semid);
        cnt++;
    }
    clean_up();
    return 0;
}

void clean_up()
{
    semctl(semid, 0, IPC_RMID, NULL);
}

void set_semaphore_val(int semid, int semnum, int val)
{
    union semun sem_un;
    sem_un.val = val;
    if (semctl(semid, semnum, SETVAL, sem_un)) {
        perror("semctl"); 
        exit(1);
    }
}

void wait_and_lock(int semid)
{
    struct sembuf sops[2];
    //index 0: writer
    sops[0].sem_num = 0;
    sops[0].sem_op = 1;
    sops[0].sem_flg = SEM_UNDO;

    //index 1: reader
    sops[1].sem_num = 1;
    sops[1].sem_op = 0;
    sops[1].sem_flg = SEM_UNDO;
    if (semop(semid, sops, 2) == -1) {
        perror("semop"); 
        exit(1);
    }
}

void unlock(int semid)
{
    struct sembuf sops;
    sops.sem_num = 0;
    sops.sem_op = -1;
    sops.sem_flg = SEM_UNDO;
    if (semop(semid, &sops, 1) == -1) {
        perror("semop"); 
        exit(1);
    }
}
