#include "headers.h"
#include "cd.h"

extern char last_working_dir[100];

void cd_implementation(char command_words[MAX_NO_COMMANDS_IN_INPUT][100], int len, char curr_dir[100], int number_of_commands)
{
    char final_wd[100];
    getcwd(final_wd, 100);

    if (command_words[1][0] == '-')
    {
        char prev[100];
        getcwd(prev, 100);
        int ch = chdir(last_working_dir);
        printf("%s\n", last_working_dir);

        if (ch < 0)
        {
            printf("Change of directory was not successful\n");
        }
        strcpy(last_working_dir, prev);
    }
    else if (command_words[1][0] == '~')
    {
        getcwd(last_working_dir, 100);
        strcpy(final_wd, curr_dir);
        if (command_words[1][1] != '\0')
        {
            strcat(final_wd, &command_words[1][1]);
            int ch = chdir(final_wd);
            if (ch < 0)
            {
                printf("Change of directory was not successful\n");
            }
        }
        int ch = chdir(final_wd);
        if (ch < 0)
        {
            printf("Change of directory was not successful\n");
        }
    }
    else if (command_words[1][0] == '.' && command_words[1][1] != '.')
    {
        getcwd(last_working_dir, 100);
        if (command_words[1][1] != '\0')
        {
            strcat(final_wd, &command_words[1][1]);
            int ch = chdir(final_wd);
            if (ch < 0)
            {
                printf("Change of directory was not successful\n");
            }
        }
    }
    else if (command_words[1][0] == '.' && command_words[1][1] == '.')
    {
        getcwd(final_wd, 100);
        strcpy(last_working_dir, final_wd);
        int str_len = strlen(final_wd);
        for (int i = str_len; i > -1; --i)
        {
            if (final_wd[i] == '/' && i != 0)
            {
                char path[100];
                strncpy(path, final_wd, i);
                path[i] = '\0';
                int ch = chdir(path);
                if (ch < 0)
                {
                    printf("Change of directory was not successful\n");
                }
                break;
            }
            else if (i == 0)
            {
                int ch = chdir("/");
                if (ch < 0)
                {
                    printf("Change of directory was not successful\n");
                }
                break;
            }
        }
    }
    else if (number_of_commands == 1)
    {
        int ch = chdir(curr_dir);
        if (ch < 0)
        {
            printf("Change of directory was not successful\n");
        }
    }
    else
    {
        getcwd(last_working_dir, 100);
        strcat(final_wd, "/");
        strcat(final_wd, command_words[1]);
        int ch = chdir(final_wd);
        if (ch < 0)
        {
            printf("Change of directory was not successful\n");
        }
    }
}
