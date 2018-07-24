#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define INPUTLEN    100

void inthandler(int s);
int main(int ac, char *av[])
{
    void    inthandler(int);
    void    quithandler(int);
    char    input[INPUTLEN];
    int nchars;
    struct sigaction sigaction_buf;
    sigset_t blocked_sig_set;

    sigemptyset(&blocked_sig_set);
    sigaddset(&blocked_sig_set, SIGQUIT);
    sigaction_buf.sa_handler = inthandler;
    sigaction_buf.sa_mask = blocked_sig_set;

    sigaction(SIGINT, &sigaction_buf, NULL);
    signal(SIGQUIT, quithandler);

    do {
        printf("\nType a message\n");
        nchars = read(0, input, (INPUTLEN-1));
        if ( nchars == -1 )
            perror("read returned an error");
        else {
            input[nchars] = '\0';
            printf("You typed: %s", input);
        }
    } 
    while( strncmp( input , "quit" , 4 ) != 0 );
    return 0;
}

void inthandler(int s)
{
    printf(" Received signal %d .. waiting\n", s );
    sleep(2);
    printf("  Leaving inthandler \n");
}

void quithandler(int s)
{
    printf(" Received signal %d .. waiting\n", s );
    sleep(3);
    printf("  Leaving quithandler \n");
}
