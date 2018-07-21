#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>

void do_pwd();
ino_t get_inode(char *file);
void print_path(ino_t inode);

int main()
{

    do_pwd();
    return 0;
}

void do_pwd()
{
    print_path(get_inode(".")); 
    printf("\n");
}

ino_t get_inode(char *file)
{
    struct stat statbuf;
    if (stat(file, &statbuf) == -1) {
        fprintf(stderr, "Cannot stat ");
        perror(file);
        exit(1);
    }
    return statbuf.st_ino;
}

void print_path(ino_t inode)
{
    struct dirent *dirent_p; 
    DIR *dir_p;
    char name[255];
    //判断是否到达根目录, 根目录的.和..指向同一个inode节点
    if (inode == get_inode("..")) {
        return; 
    }
    chdir("..");
    dir_p = opendir("."); 
    if (dir_p == NULL) {
        perror(".");
        exit(1); 
    }
    while ((dirent_p = readdir(dir_p)) != NULL) {
        if (inode == dirent_p->d_ino) {
            snprintf(name, 255, dirent_p->d_name); 
            print_path(get_inode("."));
            closedir(dir_p);
            printf("/%s", name);
            return;
        }
    }
}
