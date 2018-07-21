#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    struct winsize wbuf;
    if (ioctl(STDIN_FILENO, TIOCGWINSZ, &wbuf) != -1) {
        printf("%d rows %d cols\n", wbuf.ws_row, wbuf.ws_col); 
    }
    return 0;
}
