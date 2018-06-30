/*
 * 改进
 * 1. 读取terminal的长和宽
 * 2. 百分比显示(重定向的暂时没办法显示) 
 * 3. 输入字符不回显
 * 4. 用户输入字符后不需要按回车
 *
 * 不足
 * 1. 对包含中文的文件存在问题(读取line_len长度的数据并不刚好占据终端宽度)
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>


void do_more(FILE *fp);
int see_more();
void get_terminal_size();
int getch();

static int line_len = 0;
static int page_len = 0;

int main(int argc, char *argv[])
{
    FILE *fp; 
    get_terminal_size();
    if (argc == 1) {
        do_more(stdin); 
    } else {
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
            exit(1); 
        }
        do_more(fp); 
        fclose(fp);
    }
    return 0;
}

void get_terminal_size()
{
    struct winsize w; 
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    line_len = w.ws_col + 1;//+1是因为fgets每次读取size - 1的字符
    page_len = w.ws_row - 1;//-1是因为提示占据一行
}

void do_more(FILE *fp)
{
    int reply = 0; 
    char *line = NULL;
    int num_of_line = 0;
    FILE *fp_tty = NULL;
    int percent = 0;
    int size = 0;
    fp_tty = fopen("/dev/tty", "r");//tty表示控制终端, 可man 4 tty查看
    if (fp_tty == NULL) {
        exit(1); 
    }
    line = (char *)malloc(line_len * sizeof(char));
    if (line == NULL) {
        exit(1); 
    }
    fseek(fp, 0L, SEEK_END);
    size = ftell(fp); 
    fseek(fp, 0L, SEEK_SET);
    while (fgets(line, line_len, fp)) {
        if (num_of_line == page_len) {
            percent = (ftell(fp) - strlen(line)) * 100 / size;
            printf("\033[7mmore(%%%02d)\033[m", percent);//这里设置颜色反显，颜色对应值可参考https://gist.github.com/leesei/136b522eb9bb96ba45bd
            reply = see_more(fp_tty); 
            printf("\b\b\b\b\b\b\b\b\b\033[K");//这里\033[k是CSI序列，用来清除从光标位置到该行末尾的部分,详见https://zh.wikipedia.org/wiki/ANSI%E8%BD%AC%E4%B9%89%E5%BA%8F%E5%88%97
            if (reply == 0) {
                return; 
            }
            num_of_line -= reply;
        }
        if (fputs(line, stdout) == EOF) {
            return; 
        } 
        num_of_line++;
    }
    free(line);
    fclose(fp_tty);
}

int see_more(FILE *fp)
{
    int reply = 0;
    int c = getch(fp);
    if (c == EOF) {
        return reply; 
    }
    switch(c) {
        case 'q': 
            reply = 0;
            break;
        case '\n': 
            reply = 1;
            break;
        case ' ': 
            reply = page_len;
            break;
        default:
            reply = 0;
            break;
    } 
    return reply;
}


/*
 * 功能：读取终端输入，不回显，不用输入回车
 * 参考：https://stackoverflow.com/questions/1798511/how-to-avoid-pressing-enter-with-getchar
 */ 
int getch(FILE *fp)
{
    struct termios old_attr, new_attr;
    int ch;
    tcgetattr(fileno(fp), &old_attr);//将FILE对象转为fd, 注意此处不能直接使用STDIN_FILENO,因为使用重定向时标准输入已不再是终端
    new_attr = old_attr;
    new_attr.c_lflag &= ~(ICANON | ECHO);/
    tcsetattr(fileno(fp), TCSANOW, &new_attr);
    ch = getc(fp);
    tcsetattr(fileno(fp), TCSANOW, &old_attr);
    return ch;
}
