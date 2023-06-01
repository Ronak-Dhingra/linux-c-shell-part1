#include "headers.h"
#include "ls.h"

int alphasort(const struct dirent **a, const struct dirent **b)
{
    const char *s1 = (char *)malloc(sizeof(char));
    char string1[30];
    int x1 = 0;
    int index1 = 0;
    const char *s2 = (char *)malloc(sizeof(char));
    char string2[30];
    int x2 = 0;
    int index2 = 0;
    s1 = (*a)->d_name;
    s2 = (*b)->d_name;

    for (int i = 0; i < strlen(s1); i++)
    {
        if (s1[i] != '.')
        {
            string1[index1] = s1[i];
            ++index1;
        }
    }

    for (int i = 0; i < strlen(s2); i++)
    {
        if (s2[i] != '.')
        {
            string2[index2] = s2[i];
            ++index2;
        }
    }
    string1[index1] = '\0';
    string2[index2] = '\0';
    int x = strcasecmp(string1, string2);
    return -x;
}

int checkifFileis_executable(const char *filename)
{
    struct stat sb;
    if (stat(filename, &sb) == 0 && sb.st_mode & S_IXUSR)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_reg_file(const char *path)
{
    struct stat statbuf;
    stat(path, &statbuf);
    // test for a regular file
    if (S_ISREG(statbuf.st_mode))
        return 1;
    return 0;
}

int is_dir(const char *path)
{
    struct stat statbuf;
    stat(path, &statbuf);
    if (S_ISDIR(statbuf.st_mode))
        return 1;
    return 0;
}

void ls_print(struct dirent *dp)
{
    struct stat buffer;
    if (dp->d_type == DT_DIR)
    {
        printf("%s%s%s ", AC_BLUE, dp->d_name, AC_WHITE);
    }
    else if (stat(dp->d_name, &buffer) == 0 && buffer.st_mode & S_IXUSR)
    {
        printf("%s%s%s ", AC_GREEN, dp->d_name, AC_WHITE);
    }
    else
    {
        printf("%s ", dp->d_name);
    }
}

void file_status2(const char *filepath)
{
    struct stat buffer;
    struct passwd *owner;
    struct group *group;
    int ret_val = stat(filepath, &buffer);

    int permissions_num = 511;
    int a = 256;
    char permissions[] = "rwx";
    char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    struct tm *t;
    t = localtime(&buffer.st_mtime);
    printf("-");

    int date = t->tm_mday;
    int mon = t->tm_mon;
    int hour = t->tm_hour;
    int min = t->tm_min;
    int size = buffer.st_size;

    permissions_num = permissions_num & buffer.st_mode;
    for (int i = 0; i < 9; i++)
    {
        printf("%c", (permissions_num & a) ? permissions[i % 3] : '-');
        a = a / 2;
    }

    printf(" %ld ", buffer.st_nlink);

    owner = getpwuid(buffer.st_uid);
    group = getgrgid(buffer.st_gid);
    printf("%s %s ", owner->pw_name, group->gr_name);
    printf(" %5ld ", buffer.st_size);
    printf("%s %2d %2d:%2d ", months[mon], date, hour, min);
    // ls_print(dir);
}

void file_status(struct dirent *dir)
{
    struct stat buffer;
    struct passwd *owner;
    struct group *group;
    int ret_val = stat(dir->d_name, &buffer);

    int permissions_num = 511;
    int a = 256;
    char permissions[] = "rwx";
    char *months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

    if (dir->d_type == DT_DIR)
    {
        printf("d");
    }
    else
    {
        printf("-");
    }

    struct tm *t;
    t = localtime(&buffer.st_mtime);

    int date = t->tm_mday;
    int mon = t->tm_mon;
    int hour = t->tm_hour;
    int min = t->tm_min;
    int size = buffer.st_size;

    permissions_num = permissions_num & buffer.st_mode;
    for (int i = 0; i < 9; i++)
    {
        printf("%c", (permissions_num & a) ? permissions[i % 3] : '-');
        a = a / 2;
    }

    printf(" %ld ", buffer.st_nlink);

    owner = getpwuid(buffer.st_uid);
    group = getgrgid(buffer.st_gid);
    printf("%s %s ", owner->pw_name, group->gr_name);
    printf(" %5ld ", buffer.st_size);
    printf("%s %2d %2d:%2d ", months[mon], date, hour, min);
    ls_print(dir);
    // printf("\n");
}

void ls_implementation(char command_words[MAX_NO_COMMANDS_IN_INPUT][100], int len, char curr_dir[100], int number_of_commands)
{
    struct stat sb;
    int n;
    char present_dir[100];
    getcwd(present_dir, 100);
    int pres_len = strlen(present_dir);
    char flags[2] = {0, 0};
    char paths_to_implement[100][100];

    // Getting all flags
    for (int i = 1; i < number_of_commands; ++i)
    {
        if (command_words[i][0] == '-')
        {
            int x = strlen(command_words[i]);
            if (x == 3)
            {
                flags[0] = 1;
                flags[1] = 1;
            }
            else if (x == 2)
            {
                if (command_words[i][1] == 'a')
                {
                    flags[0] = 1;
                }
                else
                {
                    flags[1] = 1;
                }
            }
        }
        else if (command_words[i][0] == 'a' && flags[0] != 'a' && flags[1] != 'a')
        {
            int x = strlen(command_words[i]);
            if (x == 2)
            {
                flags[0] = 1;
                flags[1] = 1;
            }
            else
            {
                flags[0] = 1;
            }
        }
        else if (command_words[i][0] == 'l' && flags[0] != 'l' && flags[1] != 'l')
        {
            int x = strlen(command_words[i]);
            if (x == 2)
            {
                flags[0] = 1;
                flags[1] = 1;
            }
            else
            {
                flags[1] = 1;
            }
        }
    }

    // Paths to implemement
    int x = 0;
    for (int i = 1; i < number_of_commands; ++i)
    {
        if (command_words[i][0] != 'l' && command_words[i][0] != '-' && command_words[i][0] != 'a' && command_words[i][0] != '~')
        {
            if (pres_len >= len)
            {
                strcpy(paths_to_implement[x], curr_dir);
                strcat(paths_to_implement[x], "/");
                strcat(paths_to_implement[x], &command_words[i][0]);
                ++x;
            }
            else
            {
                strcpy(paths_to_implement[x], present_dir);
                strcat(paths_to_implement[x], "/");
                strcat(paths_to_implement[x], &command_words[i][0]);
                ++x;
            }
        }
        else if (command_words[i][0] != 'l' && command_words[i][0] != '-' && command_words[i][0] != 'a' && command_words[i][0] == '~')
        {
            if (pres_len >= len)
            {
                strcpy(paths_to_implement[x], curr_dir);
                strcat(paths_to_implement[x], &command_words[i][1]);
                ++x;
            }
            else
            {
                strcpy(paths_to_implement[x], present_dir);
                strcat(paths_to_implement[x], &command_words[i][1]);
                ++x;
            }
        }
    }
    if (x == 0)
    {
        strcpy(paths_to_implement[0], ".");
        ++x;
    }
    int flag_for_printing_dir_name = 0;
    if (x > 1)
    {
        flag_for_printing_dir_name = 1;
    }

    // Implementation of cases with different flags

    if (flags[0] == 0 && flags[1] == 0)
    {
        struct dirent **namelist;

        // No flags
        // Implement all paths
        DIR *dir;
        struct dirent *dp;
        struct stat buffer;
        for (int i = 0; i < x; ++i)
        {
            dir = opendir(paths_to_implement[i]);
            if (dir == NULL)
            {
                if (!(is_reg_file(paths_to_implement[i])))
                {
                    printf("Cannot open directory %s\n", paths_to_implement[i]);
                }
                else
                {
                    if (checkifFileis_executable(paths_to_implement[i]))
                    {
                        printf("%s%s%s \n", AC_GREEN, &paths_to_implement[i][len + 1], AC_WHITE);
                    }
                    else
                    {
                        printf("%s \n", &paths_to_implement[i][len + 1]);
                    }
                }
            }
            else
            {
                n = scandir(paths_to_implement[i], &namelist, NULL, alphasort);
                while (n--)
                {
                    if (namelist[n]->d_name[0] != '.')
                    {
                        ls_print(namelist[n]);
                        printf("\n");
                        free(namelist[n]);
                    }
                }
                free(namelist);
                // printf("\n");
            }
            closedir(dir);
        }
    }
    else if (flags[0] == 1 && flags[1] == 0)
    {
        struct dirent **namelist;

        // only a flag is there
        // Implement all paths
        DIR *dir;
        struct dirent *dp;
        struct stat buffer;
        for (int i = 0; i < x; ++i)
        {
            dir = opendir(paths_to_implement[i]);
            if (dir == NULL)
            {
                if (!(is_reg_file(paths_to_implement[i])))
                {
                    printf("Cannot open directory %s\n", paths_to_implement[i]);
                }
                else
                {
                    if (checkifFileis_executable(paths_to_implement[i]))
                    {
                        printf("%s%s%s \n", AC_GREEN, &paths_to_implement[i][len + 1], AC_WHITE);
                    }
                    else
                    {
                        printf("%s \n", &paths_to_implement[i][len + 1]);
                    }
                }
            }
            else
            {
                n = scandir(paths_to_implement[i], &namelist, NULL, alphasort);
                while (n--)
                {
                    ls_print(namelist[n]);
                    printf("\n");
                    free(namelist[n]);
                }
                free(namelist);
                // printf("\n");
            }
            closedir(dir);
        }
    }
    else if (flags[0] == 0 && flags[1] == 1)
    {
        // only l flag
        // Implement all paths
        DIR *dir;
        struct dirent *dp;
        for (int i = 0; i < x; ++i)
        {
            char path[1000];
            strcpy(path, present_dir);
            strcat(path, "/");
            strcat(path, paths_to_implement[i]);
            dir = opendir(paths_to_implement[i]);
            if (dir == NULL)
            {
                if (!(is_reg_file(paths_to_implement[i])))
                {
                    printf("Cannot open directory %s\n", paths_to_implement[i]);
                }
                else
                {
                    file_status2(paths_to_implement[i]);
                    if (checkifFileis_executable(paths_to_implement[i]))
                    {
                        printf("%s%s%s \n", AC_GREEN, &paths_to_implement[i][len + 1], AC_WHITE);
                    }
                    else
                    {
                        printf("%s \n", &paths_to_implement[i][len + 1]);
                    }
                }
            }
            else
            {
                struct dirent **namelist;
                int ret = chdir(paths_to_implement[i]);
                n = scandir(".", &namelist, NULL, alphasort);
                struct stat bf;
                if (flag_for_printing_dir_name == 1)
                {
                    printf("\n%s\n", &paths_to_implement[i][len + 1]);
                }
                printf("total ");
                stat(present_dir, &bf);
                long long int no_of_blocks = 0;
                int count = 0;
                while (count < n)
                {
                    char *name = namelist[count]->d_name;
                    if (name[0] != '.')
                    {
                        struct stat bf_temp;
                        stat(name, &bf_temp);
                        no_of_blocks = no_of_blocks + bf_temp.st_blocks;
                    }
                    ++count;
                }
                no_of_blocks = no_of_blocks / 2;
                printf("%lld\n", no_of_blocks);
                for (int j = 0; j < n; ++j)
                {
                    if (namelist[j]->d_name[0] != '.')
                    {
                        file_status(namelist[j]);
                        printf("\n");
                    }
                    free(namelist[j]);
                }
                free(namelist);
            }
            chdir(present_dir);
        }
    }
    else if (flags[0] == 1 && flags[1] == 1)
    {
        struct dirent **namelist;

        // No flags
        // Implement all paths
        DIR *dir;
        struct direct *dp;
        struct stat buffer;
        for (int i = 0; i < x; ++i)
        {
            dir = opendir(paths_to_implement[i]);
            if (dir == NULL)
            {
                if (!(is_reg_file(paths_to_implement[i])))
                {
                    printf("Cannot open directory %s\n", paths_to_implement[i]);
                }
                else
                {
                    file_status2(paths_to_implement[i]);
                    if (checkifFileis_executable(paths_to_implement[i]))
                    {
                        printf("%s%s%s \n", AC_GREEN, &paths_to_implement[i][len + 1], AC_WHITE);
                    }
                    else
                    {
                        printf("%s \n", &paths_to_implement[i][len + 1]);
                    }
                }
            }
            else
            {
                int ret = chdir(paths_to_implement[i]);
                n = scandir(".", &namelist, NULL, alphasort);
                struct stat bf;
                if (flag_for_printing_dir_name == 1)
                {
                    printf("\n%s\n", &paths_to_implement[i][len + 1]);
                }
                printf("total ");
                stat(present_dir, &bf);
                long long int no_of_blocks = 0;
                int count = 0;
                while (count < n)
                {
                    char *name = namelist[count]->d_name;
                    struct stat bf_temp;
                    stat(name, &bf_temp);
                    no_of_blocks = no_of_blocks + bf_temp.st_blocks;
                    ++count;
                }
                no_of_blocks = no_of_blocks / 2;
                printf("%lld\n", no_of_blocks);
                while (n--)
                {
                    file_status(namelist[n]);
                    printf("\n");
                    free(namelist[n]);
                }
                free(namelist);
                // printf("\n");
            }
            closedir(dir);
            chdir(present_dir);
        }
    }
}
