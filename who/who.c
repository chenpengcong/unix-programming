#include <utmp.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    struct utmp login_record;
    int utmp_fd = open(UTMP_FILE, O_RDONLY);
    if (utmp_fd == -1) {
        perror(UTMP_FILE);
        exit(1); 
    }
    while(read(utmp_fd, (void *)&login_record, sizeof(struct utmp)) > 0) {
        if (login_record.ut_type != USER_PROCESS) {
            continue; 
        }
        time_t time_val = login_record.ut_tv.tv_sec;
        printf("%-9.9s", login_record.ut_user);  
        printf("%-9.9s", login_record.ut_line);
        printf("%s", ctime(&time_val));
        //printf("%s", ctime((time_t*)(&(login_record.ut_tv.tv_sec))));//此处打印是错的，详见<收获.md>
        printf("\n");
    }
    close(utmp_fd);
    return 0;
}
