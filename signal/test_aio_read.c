#include <aio.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
//#include <aiocb.h>

void on_input(int signum);

static struct aiocb aiocb_buf;

int main()
{
    char buf[1];
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
    char *c_p = (char *)aiocb_buf.aio_buf;
    printf("receive %c\n", *c_p);
    aio_read(&aiocb_buf);
}
