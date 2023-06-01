#include "headers.h"
#include "echo.h"
#include "printdir.h"
#include "history.h"
#include "cd.h"
#include "ls.h"
#include "discover.h"
/*
            NOTE
curr_dir is the inital directory from which the code is executed not the present directory
*/

char last_working_dir[100];
int no_of_background = 1;
int spec6_output_index = 0;
int spec6_output_traverser = 0;
int bg_processes[1000];
char bg_command[1000][100];
char spec6_output[1000][1000];
int time_flag = 0;

void background_handler()
{
    int status;
    int x = 0;
    int p_id = waitpid(-1, &status, WNOHANG);

    if (p_id > 0)
    {
        // printf("hello\n");
        while (bg_processes[x] != p_id)
        {
            ++x;
        }
        // printf("hii\n");
        if (WIFSTOPPED(status))
        {
            return;
        }

        sprintf(spec6_output[spec6_output_index++], "%s with pid %d exited %s\n", bg_command[x], bg_processes[x], WIFEXITED(status) ? "normally" : "abnormally");
        // printf("%s with pid %d exited %s\n", bg_command[x], bg_processes[x], WIFEXITED(status) ? "normally" : "abnormally");
        return;
    }
}

void init_shell()
{
    clear();
}

void pwd_implementation()
{
    char current_wd[100];
    getcwd(current_wd, 100);
    printf("%s\n", current_wd);
}

void pinfo(char command_words[MAX_NO_COMMANDS_IN_INPUT][100], int len, char curr_dir[100], int number_of_commands)
{
    int pid;
    char path[1000];
    char exec_filepath[1000];
    if (number_of_commands == 1)
    {
        pid = getpid();
        sprintf(path, "/proc/%d/stat", pid);
        sprintf(exec_filepath, "/proc/%d/exe", pid);
    }
    else
    {
        pid = atoi(command_words[1]);
        strcpy(path, "/proc/");
        strcat(path, command_words[1]);
        strcpy(exec_filepath, path);
        strcat(path, "/stat");
        strcat(exec_filepath, "/exe");
    }
    char pinfo[1000];
    FILE *procfile;
    procfile = fopen(path, "r");
    if (procfile == NULL)
    {
        perror("Couldn't open procfile\n");
        return;
    }
    fread(pinfo, 1000, 1, procfile);
    fclose(procfile);

    char tokens_pinfo[1000][100];
    char *rest = pinfo;
    char *token;
    int x = 0;

    while ((token = strtok_r(rest, " ", &rest)))
    {
        strcpy(tokens_pinfo[x], token);
        ++x;
    }
    if (strcmp(tokens_pinfo[4], tokens_pinfo[7]) == 0 )
    {
        strcat(tokens_pinfo[2], "+");
    }
    printf("pid : %d\n", pid);
    printf("process Status : %s\n", tokens_pinfo[2]);
    printf("memory : %s\n", tokens_pinfo[22]);

    // For the executable path
    char exec_path[4096] = {'\0'};
    int readlink_ret = readlink(exec_filepath, exec_path, 4096);

    if (readlink_ret == -1)
    {
        printf("Zombie process: Execpath doesn't exist\n");
    }
    else
    {
        printf("executable path : %s\n", exec_path);
    }
}

void executer(char *command_words[100])
{
    if (execvp(command_words[0], command_words) < 0)
    {
        printf("Error: Command not found : %s\n", command_words[0]);
    }
}

void executeInputcommand(char command_words[100][100], int len, char curr_dir[100], int number_of_commands)
{
    if (strcmp(command_words[0], "echo") == 0)
    {
        echo_implementation(command_words, number_of_commands);
    }

    else if (strcmp(command_words[0], "pwd") == 0)
    {
        pwd_implementation();
    }

    else if (strcmp(command_words[0], "cd") == 0)
    {
        if (number_of_commands <= 2)
        {
            cd_implementation(command_words, len, curr_dir, number_of_commands);
        }
        else
        {
            printf("Too many arguments\n");
        }
    }
    else if (strcmp(command_words[0], "history") == 0)
    {
        history_implementation(curr_dir);
    }
    else if (strcmp(command_words[0], "ls") == 0)
    {
        ls_implementation(command_words, len, curr_dir, number_of_commands);
    }
    else if (strcmp(command_words[0], "pinfo") == 0)
    {
        pinfo(command_words, len, curr_dir, number_of_commands);
    }
    else if (strcmp(command_words[0], "discover") == 0)
    {
        discover(command_words, len, curr_dir, number_of_commands);
    }
    else
    {
        printf("Invalid Command\n");
    }
}

void space_parser(char command[100], int len, char curr_dir[100])
{
    int number_of_commands = 0;
    char command_words[MAX_NO_COMMANDS_IN_INPUT][100];
    char *token;
    char *rest = command;

    while ((token = strtok_r(rest, " ", &rest)))
    {
        strcpy(command_words[number_of_commands], token);
        ++number_of_commands;
    }
    executeInputcommand(command_words, len, curr_dir, number_of_commands);
}

void background_proc(char command[100], int len, char curr_dir[100])
{
    char *command_words[100];

    char *token;
    char *rest = command;
    int no_of_commands = 0;

    while ((token = strtok_r(rest, " ", &rest)))
    {
        command_words[no_of_commands] = token;
        ++no_of_commands;
    }
    command_words[no_of_commands] = NULL;

    pid_t ret_value = fork();
    int status;

    if (ret_value == 0)
    {
        setpgid(0,0);
        if (execvp(command_words[0], command_words) < 0)
        {
            printf("Error: Command not found : %s\n", command_words[0]);
        }
    }
    else
    {
        waitpid(ret_value, &status, WNOHANG);
        bg_processes[no_of_background] = ret_value;
        strcpy(bg_command[no_of_background], command_words[0]);
        printf("[%d] %d\n", no_of_background, ret_value);
        ++no_of_background;
    }
}

void foreground_proc(char command[100], int len, char curr_dir[100])
{
    char *command_words[100];
    time_t start;
    start = time(NULL);

    char *token;
    char *rest = command;
    int no_of_commands = 0;

    while ((token = strtok_r(rest, " ", &rest)))
    {
        command_words[no_of_commands] = token;
        ++no_of_commands;
    }
    command_words[no_of_commands] = NULL;

    pid_t ret_value = fork();
    int status;

    if (ret_value == 0)
    {
        if (execvp(command_words[0], command_words) < 0)
        {
            printf("Error: Command not found : %s\n", command_words[0]);
        }
    }
    else
    {
        waitpid(ret_value, &status, 0);
        time_t end;
        end = time(NULL);
        time_flag = end - start;
    }
}

void Readinputcommand_amp(char command[100], int len, char curr_dir[100])
{
    char *token;
    char *rest = command;

    int flag = 0;
    int str_len = strlen(command);

    int flag2 = 0;

    for (int i = 0; command[i] != '\0'; ++i)
    {
        if (command[i] == '&')
        {
            flag2 = 1;
            break;
        }
    }

    if (flag2 != 1)
    {
        space_parser(command, len, curr_dir);
        return;
    }

    if (command[str_len - 1] == '&')
    {
        flag = 1;
    }

    while ((token = strtok_r(rest, "&", &rest)))
    {
        if (rest[0] == '\0' && flag == 0)
        {
            foreground_proc(token, len, rest);
        }
        else
        {
            background_proc(token, len, curr_dir);
        }
    }
}

void Readinputcommand(char input_command[MAX_COMMAND_LEN], int len, char curr_dir[100])
{
    char command[100];
    char *token;
    char *rest = input_command;

    while ((token = strtok_r(rest, ";", &rest)))
    {
        strcpy(command, token);
        Readinputcommand_amp(command, len, curr_dir);
    }
}

int main()
{
    signal(SIGCHLD, background_handler);
    init_shell();
    char curr_dir[CURR_DIR_LEN];
    getcwd(curr_dir, 100);
    int len = strlen(curr_dir);
    strcpy(last_working_dir, curr_dir);
    char present_dir[100];
    char prev_command[100];
    int flag = 0;
    char last_command[100];

    while (1)
    {
        getcwd(present_dir, 100);
        if (strlen(present_dir) >= strlen(curr_dir))
        {
            print_directory(len);
        }
        else
        {
            print_directory2(len);
        }
        char input_command[MAX_COMMAND_LEN];
        gets(input_command);
        if (strcmp(input_command, last_command) != 0)
        {
            add_to_History(input_command, curr_dir, len);
        }
        Readinputcommand(input_command, len, curr_dir);
        for (int i = spec6_output_traverser; spec6_output[i][0] != '\0'; ++i)
        {
            printf("%s", spec6_output[spec6_output_traverser++]);
        }
        strcpy(last_command, input_command);
    }
}