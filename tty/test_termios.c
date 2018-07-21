#include <termios.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    char c;
    struct termios termios_buf;
    tcgetattr(STDIN_FILENO, &termios_buf);
    termios_buf.c_lflag &= ~ECHO; 
    tcsetattr(STDIN_FILENO, TCSANOW, &termios_buf);
    return 0;
}
