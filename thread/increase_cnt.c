#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define MUTEX 1

int cnt = 0;
pthread_mutex_t fastmutex = PTHREAD_MUTEX_INITIALIZER;
void *increase_cnt(void *arg);
int main()
{
    pthread_t thread1, thread2, thread3;
    pthread_create(&thread1, NULL, increase_cnt, NULL);
    pthread_create(&thread2, NULL, increase_cnt, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("%d\n", cnt);
    return 0;
}

void *increase_cnt(void *arg)
{
    int i;
    for (i = 0;i < 10000;i++) {
#if defined(MUTEX) && MUTEX == 1
        pthread_mutex_lock(&fastmutex);
#endif
        cnt = cnt + 1; 
#if defined(MUTEX) && MUTEX == 1
        pthread_mutex_unlock(&fastmutex);
#endif
    }
}
