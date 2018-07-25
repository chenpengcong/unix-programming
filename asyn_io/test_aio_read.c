#include <aio.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>

void on_input(int signum);
void init_terminal(int flag);
static struct aiocb aiocb_buf;

int main()
{
    char buf[1];
    init_terminal(0);
    aiocb_buf.aio_fildes = 0;
    aiocb_buf.aio_offset = 0;
    aiocb_buf.aio_buf = buf;
    aiocb_buf.aio_nbytes = 1;
    aiocb_buf.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
    aiocb_buf.aio_sigevent.sigev_signo = SIGIO;
    aio_read(&aiocb_buf);

    signal(SIGIO, on_input);
    while (1) {
        pause(); 
    }
    return 0;
}

void on_input(int signum)
{
    int error_status;
    int size;
    char *c_p = (char *)aiocb_buf.aio_buf;
    error_status = aio_error(&aiocb_buf); 
    if (error_status != 0) {
        perror("reading failed");
    } else {
        if (aio_return(&aiocb_buf) == 1) {
            if (*c_p == 'q' || *c_p == 'Q') {
                init_terminal(1); 
                exit(0);
            } else {
                printf("receive %c\n", *c_p); 
            }
        }
        
    }
    aio_read(&aiocb_buf);
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

