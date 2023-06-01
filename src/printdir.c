#include "headers.h"
#include "printdir.h"

extern int time_flag;

void print_directory(int len)
{
    char username[100];
    getlogin_r(username, 100);
    char sys_name[SYS_NAME_LEN];
    gethostname(sys_name, SYS_NAME_LEN);
    char curr_dir[CURR_DIR_LEN];
    getcwd(curr_dir, 100);
    if (time_flag > 1)
    {
        printf("\033[0;31m%s@%s:\033[0;34m~[took %ds]%s>\033[0;37m", username, sys_name, time_flag, &curr_dir[len]);
        time_flag = 0;
    }
    else
    {
        printf("\033[0;31m%s@%s:\033[0;34m~%s>\033[0;37m", username, sys_name, &curr_dir[len]);
    }
}

void print_directory2(int len)
{
    char username[100];
    getlogin_r(username, 100);
    char sys_name[SYS_NAME_LEN];
    gethostname(sys_name, SYS_NAME_LEN);
    char curr_dir[CURR_DIR_LEN];
    getcwd(curr_dir, 100);
    if (time_flag > 1)
    {
        printf("\033[0;31m%s@%s:\033[0;34m~[took %ds]%s>\033[0;37m", username, sys_name, time_flag, curr_dir);
    }
    else
    {
        printf("\033[0;31m%s@%s:\033[0;34m~%s>\033[0;37m", username, sys_name, curr_dir);
    }
}