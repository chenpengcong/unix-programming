#include <sys/time.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void count_down(int signum);
int set_timer(int msec);

int main()
{
    signal(SIGALRM, count_down);
    if (set_timer(1000) == -1) {
        perror("set_timer"); 
        exit(1);
    }
    while (1) {
        pause(); 
    }
    return 0;
}

void count_down(int signum)
{
    static int cnt = 10;
    printf("cnt:%d\n", cnt);
    cnt--;
    if (cnt < 0) {
        printf("Done!\n"); 
        exit(1);
    }
}

int set_timer(int msec)
{
    struct itimerval itimerval_buf;

    long sec = msec / 1000;
    long usec = (msec % 1000) * 1000;
     

    itimerval_buf.it_value.tv_sec = sec;
    itimerval_buf.it_value.tv_usec = usec;

    itimerval_buf.it_interval.tv_sec = sec;
    itimerval_buf.it_interval.tv_usec = usec;

    return setitimer(ITIMER_REAL, &itimerval_buf, NULL);
}
