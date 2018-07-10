#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void do_ls(char *dir_name);
void mode_to_letter(mode_t mode, char *mode_str);
char* uid_to_name(uid_t uid);
char* gid_to_name(gid_t gid);
void reset_working_directory();


int main(int argc, char *argv[])
{
    int i = 1;
    if (argc == 1) {
        do_ls("."); 
    } else {
        while(i < argc) {
            do_ls(argv[i]); 
            i++;
            printf("\n");
        } 
    }
    return 0;
}

void do_ls(char *dir_name)
{
    struct dirent *dir_entry = NULL;
    struct stat stat_buf;
    char mode_str[11] = {0};
    time_t t = 0;
    int chdir_res = 0;
    char file_path[NAME_MAX];
    DIR *dir = opendir(dir_name);
    if (dir == NULL) {
        perror("can not open directory"); 
        exit(1);
    }
    while ((dir_entry = readdir(dir)) != NULL) {
        if (dir_entry->d_name[0] == '.') {
            continue; 
        }
        snprintf(file_path, NAME_MAX, "%s/%s", dir_name, dir_entry->d_name);//如果读取的不是当前工作目录下的文件，需要加上所在路径
        //from stat(2) man page:lstat() is identical to stat(), except that if pathname  is  a  symbolic  link,  then it returns information about the link itself, not the file that it refers to.
        if (lstat(file_path, &stat_buf) == -1) {
            perror(dir_entry->d_name); 
            exit(1);
        }
        mode_to_letter(stat_buf.st_mode, mode_str);
        printf("%s %d %s %s %d %.12s %s\n", mode_str, stat_buf.st_nlink, uid_to_name(stat_buf.st_uid), gid_to_name(stat_buf.st_gid), stat_buf.st_size, ctime((time_t *)&(stat_buf.st_mtime)) + 4, dir_entry->d_name);  
    }
    closedir(dir);
    if (errno != 0) {
        perror("readdir error"); 
        exit(1);
    }
}


char* uid_to_name(uid_t uid)
{
    struct passwd *passwd_p = getpwuid(uid);
    static char name[10] = {0};
    if (passwd_p == NULL) {
        snprintf(name, 10, "%d", uid); 
        return name;
    } else {
        return passwd_p->pw_name;
    }
}

char* gid_to_name(gid_t gid)
{
    struct group *group_p = getgrgid(gid); 
    static char name[10] = {0};
    if (group_p == NULL) {
        snprintf(name, 10, "%d", gid); 
        return name;
    } else {
        return group_p->gr_name; 
    }
}

/*
 * 文件类型对应的字符可以参考维基百科https://en.wikipedia.org/wiki/Unix_file_types
 * 文件类型和模式参考man 7 inode
 */
void mode_to_letter(mode_t mode, char *mode_str)
{
    memset(mode_str, '-', 10);
    if (S_ISREG(mode)) {
        mode_str[0] = '-'; 
    } else if (S_ISDIR(mode)) {
        mode_str[0] = 'd'; 
    } else if (S_ISLNK(mode)) {
        mode_str[0] = 'l';
    } else if (S_ISBLK(mode)) {
        mode_str[0] = 'b'; 
    } else if (S_ISCHR(mode)) {
        mode_str[0] = 'c'; 
    } else if (S_ISFIFO(mode)) {
        mode_str[0] = 'p'; 
    } else if (S_ISSOCK(mode)) {
        mode_str[0] = 's'; 
    }

    if (mode & S_IRUSR) {
        mode_str[1] = 'r'; 
    } 
    if (mode & S_IWUSR) {
        mode_str[2] = 'w'; 
    } 
    if (mode & S_IXUSR) {
        mode_str[3] = 'x'; 
    } 

    if (mode & S_IRGRP) {
        mode_str[4] = 'r'; 
    } 
    if (mode & S_IWGRP) {
        mode_str[5] = 'w'; 
    } 
    if (mode & S_IXGRP) {
        mode_str[6] = 'x'; 
    }

    if (mode & S_IROTH) {
        mode_str[7] = 'r'; 
    } 
    if (mode & S_IWOTH) {
        mode_str[8] = 'w'; 
    } 
    if (mode & S_IXOTH) {
        mode_str[9] = 'x'; 
    }
}
