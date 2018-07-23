#include <unistd.h>
#include <stdio.h>
#include <signal.h>

void wakeup(int signum)
{
    printf("received alarm signal\n");
}

int main()
{
    signal(SIGALRM, wakeup);
    alarm(4);
    pause();
    return 0;
}
