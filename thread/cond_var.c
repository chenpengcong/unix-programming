#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

static int is_signaled = 0;
pthread_mutex_t fastmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* signal_thread(void *arg);
void* wait_thread(void *arg);
int main()
{
    pthread_t signal_id; 
    pthread_t wait_id; 
    pthread_create(&signal_id, NULL, signal_thread, NULL);
    pthread_create(&wait_id, NULL, wait_thread, NULL);
    while (1) {
        sleep(3); 
    }
    return 0;
}


void* signal_thread(void *arg)
{
    while (1) {
        pthread_mutex_lock(&fastmutex);
        while (!is_signaled) {
            is_signaled = 1;
            pthread_cond_signal(&cond);
            printf("send signal\n");
        }
        pthread_mutex_unlock(&fastmutex);
        sleep(2);
    }
    
}

void* wait_thread(void *arg)
{
    while (1) {
        pthread_mutex_lock(&fastmutex); 
        while (!is_signaled) {
             pthread_cond_wait(&cond, &fastmutex); 
        }
        printf("received signal\n");
        is_signaled = 0;
        pthread_mutex_unlock(&fastmutex);
    }
}
