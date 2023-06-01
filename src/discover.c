#include "headers.h"
#include "discover.h"
#include "ls.h"

void find_all_files(char directory[1000], int d_flag, int f_flag, int len)
{
    struct dirent **namelist;
    int n;
    int str_len = strlen(directory);
    // printf("%d %d\n", str_len, len);

    n = scandir(directory, &namelist, 0, alphasort);

    for (int i = 0; i < n; ++i)
    {
        // printf("Im in the loop\n");
        char *name = namelist[i]->d_name;
        // printf("names : %s\n", name);
        char path[1000];
        strcpy(path, directory);
        strcat(path, "/");
        strcat(path, name);
        if (is_reg_file(path) == 0 && strcmp(name, ".") != 0 && strcmp(name, "..") != 0)
        {
            if (d_flag == 1)
            {
                if (name[0] != '.')
                {
                    // printf("hello");
                    // if (path[len] != '.')
                    char rel_path[1000];
                    if (path[len] != '.')
                    {
                        strcpy(rel_path, "./");
                        strcat(rel_path, &path[len]);
                        printf("%s\n", rel_path);
                    }
                    else
                    {
                        printf("%s\n", &path[len]);
                    }
                }
            }
            else if (d_flag == 0 && f_flag == 0)
            {
                // printf("hii");
                // printf("%s\n", &path[len]);
                // if (path[len] != '.')
                if (name[0] != '.')
                {
                    // printf("hello");
                    // if (path[len] != '.')
                    char rel_path[1000];
                    if (path[len] != '.')
                    {
                        strcpy(rel_path, "./");
                        strcat(rel_path, &path[len]);
                        printf("%s\n", rel_path);
                    }
                    else
                    {
                        printf("%s\n", &path[len]);
                    }
                }
            }
            find_all_files(path, d_flag, f_flag, len);
        }
        else if (is_reg_file(path) == 1)
        {
            // printf("Im in file state\n");
            // char a = path[str_len];
            // printf("%s\n", &path[len]);
            // printf("%s\n",&path[str_len]);
            // printf("%c\n", a);
            if (f_flag == 1 && path[str_len + 1] != '.')
            {
                // char rel_path[1000];
                // strcpy(rel_path, "./");
                // strcat(rel_path, &path[str_len + 1]);
                // printf("%s\n", rel_path);
                if (path[len] != '.')
                {
                    printf("./%s\n", &path[len]);
                }
                else
                {
                    printf("%s\n", &path[len]);
                }
            }
            else if (d_flag == 0 && f_flag == 0 && path[str_len + 1] != '.')
            {
                // printf("rltbrkm\n");
                if (path[len] != '.')
                {
                    printf("./%s\n", &path[len]);
                }
                else
                {
                    printf("%s\n", &path[len]);
                }
                // // char rel_path[1000];
                // // strcpy(rel_path, "./");
                // // strcat(rel_path, &path[str_len + 1]);
                // printf("%s\n", rel_path);
            }
        }
    }
}

void search_for_file(char file[100], char directory[1000], int len)
{
    struct dirent **namelist;
    int n;

    n = scandir(directory, &namelist, 0, alphasort);

    for (int i = 0; i < n; ++i)
    {
        char *name = namelist[i]->d_name;
        char path[1000];
        strcpy(path, directory);
        strcat(path, "/");
        strcat(path, name);
        if (is_dir(path) == 1 && strcmp(name, ".") != 0 && strcmp(name, "..") != 0)
        {
            search_for_file(file, path, len);
        }
        else if (is_reg_file(path) == 1)
        {
            // printf("%s\n", name);
            if (strcmp(name, file) == 0)
            {
                if (path[len] != '.')
                {
                    printf("./%s\n", &path[len]);
                    break;
                }
                else
                {
                    printf("%s\n", &path[len]);
                }
            }
        }
    }
}

void discover(char command_words[MAX_NO_COMMANDS_IN_INPUT][100], int len, char curr_dir[100], int no_of_commands)
{
    char present_dir[100];
    getcwd(present_dir, 100);
    int pres_len = strlen(present_dir);
    char flags[2] = {0, 0};

    // Getting the flags to implement

    for (int i = 1; i < no_of_commands; ++i)
    {
        if (command_words[i][0] == '-')
        {
            int x = strlen(command_words[i]);
            if (x == 2)
            {
                if (command_words[i][1] == 'd')
                {
                    flags[0] = 1;
                }
                else if (command_words[i][1] == 'f')
                {
                    flags[1] = 1;
                }
            }
        }
        else if (strcmp(command_words[i], "d") == 0)
        {
            flags[0] = 1;
        }
        else if (strcmp(command_words[i], "f") == 0)
        {
            flags[1] = 1;
        }
    }

    // paths and directories to implement

    char directory[1000];
    char file[100];
    int flag_for_file = 0;
    strcpy(directory, present_dir);

    if (command_words[1][0] == '~')
    {
        strcpy(directory, curr_dir);
        strcat(directory, &command_words[1][1]);
    }
    else if (strcmp(command_words[1], "..") == 0)
    {
        strcat(directory, "/..");
    }
    else if (command_words[1][0] == '.' && command_words[1][1] != '.')
    {
        strcat(directory, "/");
        strcat(directory, command_words[1]);
    }
    // printf("%s\n", directory);
    // return;
    //  for file

    for (int i = 0; i < no_of_commands; ++i)
    {
        if (command_words[i][0] == '"')
        {
            flag_for_file = 1;
            for (int j = 1; command_words[i][j] != '"'; ++j)
            {
                file[j - 1] = command_words[i][j];
            }
            break;
        }
    }

    if (flag_for_file == 1)
    {
        search_for_file(file, directory, pres_len + 1);
    }
    else
    {
        // printf("Im here\n");
        find_all_files(directory, flags[0], flags[1], pres_len + 1);
    }
}
