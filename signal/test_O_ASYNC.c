#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <termios.h>

void on_input(int signum);

void init_terminal(int flag);
int main()
{
    int status_flag;
    char c;
    struct termios old_attr, new_attr;
    int ch;
    init_terminal(0);
    
    fcntl(0, F_SETOWN, getpid());
    status_flag = fcntl(0, F_GETFL);
    status_flag |= O_ASYNC;
    fcntl(0, F_SETFL, O_ASYNC);
    
    signal(SIGIO, on_input);
    while (1) {
        pause(); 
    }
    return 0;
}

void on_input(int signum)
{
    char c = getchar();
    if (c == 'q' || c == 'Q') {
        printf("done\n"); 
        init_terminal(1);
        exit(1);
    }
    printf("receive %c\n", c);
}


void init_terminal(int flag)
{
    static struct termios old_attr, new_attr;
    //无须回车，不回显
    if (flag == 0) {
        tcgetattr(0, &old_attr);
        new_attr = old_attr;
        new_attr.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(0, TCSANOW, &new_attr);
    } else {
        //还原
        tcsetattr(0, TCSANOW, &old_attr);
    }
}
