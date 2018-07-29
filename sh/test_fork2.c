#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
    FILE *fp;
    int pid;
    char msg1[] = "Test 1 2 3...\n";
    char msg2[] = "Hello, hello\n";
    if ((fp = fopen("testfile2", "w")) == NULL) {
        perror("testfile2");
        return 0; 
    }
    fprintf(fp, "%s", msg1);
    //fflush(fp);
    if (fork() == -1) {
        perror("fork");
        return 0; 
    }
    fprintf(fp, "%s", msg2);
    //fflush(fp);
    fclose(fp);
    return 0;
}
