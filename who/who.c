#include <utmp.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE 16
#define UTMP_SIZE (sizeof(struct utmp))

int main()
{
    int i;
    struct utmp login_records[BUFFER_SIZE];
    ssize_t num = 0;
    int utmp_fd = open(UTMP_FILE, O_RDONLY);
    if (utmp_fd == -1) {
        perror(UTMP_FILE);
        exit(1); 
    }
    while ((num = read(utmp_fd, (void *)login_records, BUFFER_SIZE * UTMP_SIZE)) > 0) {
        for (i = 0;i < num / UTMP_SIZE;i++) {
            if (login_records[i].ut_type != USER_PROCESS) {
                continue; 
            }
            time_t time_val = login_records[i].ut_tv.tv_sec;
            printf("%-9.9s", login_records[i].ut_user);  
            printf("%-9.9s", login_records[i].ut_line);
            printf("%s", ctime(&time_val));
            //printf("%s", ctime((time_t*)(&(login_records[i].ut_tv.tv_sec))));//此处打印是错的，详见<收获.md>
            printf("\n");    
        } 
        
    }
    close(utmp_fd);
    return 0;
}
