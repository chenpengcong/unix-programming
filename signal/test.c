#include <termios.h>
#include <stdio.h>
int getch()
{
    struct termios old_attr, new_attr;
    int ch;
    tcgetattr(0, &old_attr);
    new_attr = old_attr;
    new_attr.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &new_attr);
    ch = getchar();
    return ch;
}


int main()
{
    char c = getch();
    printf("c: %c\n", c);
}
