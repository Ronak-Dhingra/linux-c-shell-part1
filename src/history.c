#include "headers.h"
#include "history.h"

int getHistory(char curr_dir[100], char *commands[100])
{
    for (int i = 0; i < 21; ++i)
    {
        commands[i] = (char *)malloc(sizeof(char) * 100);
    }
    int no_of_commands_stored_in_history = 0;
    long unsigned size = 1000;

    FILE *history_file = fopen("/tmp/.histry5", "r");

    if (history_file)
    {
        for (int i = 0; i < 1000; ++i)
        {
            char *temp_com;
            temp_com = (char *)malloc(100 * sizeof(char));

            if (getline(&temp_com, &size, history_file) > 0)
            {
                if (strcmp(temp_com, "\n") != 0)
                {
                    strcpy(commands[no_of_commands_stored_in_history++], temp_com);
                }
            }
            else
            {
                break;
            }
        }
        fclose(history_file);
    }
    return no_of_commands_stored_in_history;
}

void history_implementation(char curr_dir[100])
{
    char *commands[21];
    for (int i = 0; i < 21; ++i)
    {
        commands[i] = (char *)malloc(sizeof(char) * 100);
    }
    int no_of_commands_stored_in_history = getHistory(curr_dir, commands);

    if (no_of_commands_stored_in_history <= 10)
    {
        for (int i = 0; i < no_of_commands_stored_in_history; ++i)
        {
            printf("%s", commands[i]);
        }
    }
    else
    {
        for (int i = no_of_commands_stored_in_history - 10; i < no_of_commands_stored_in_history; ++i)
        {
            printf("%s", commands[i]);
        }
    }
    return;
}

void add_to_History(char command[100], char curr_dir[100], int len)
{
    char *commands[21];
    for (int i = 0; i < 21; ++i)
    {
        commands[i] = (char *)malloc(sizeof(char) * 100);
    }
    int no_of_commands_stored_in_history = getHistory(curr_dir, commands);
    if (no_of_commands_stored_in_history > 0)
    {
        char temp[100];
        strcpy(temp, command);
        strcat(temp, "\n");
        int x = strcmp(temp, commands[no_of_commands_stored_in_history - 1]);

        if (x == 0)
        {
            return;
        }
    }
    FILE *history_file = fopen("/tmp/.histry5", "w+");
    if (history_file)
    {
        if (no_of_commands_stored_in_history < 20)
        {
            if (no_of_commands_stored_in_history == 0)
            {
                fprintf(history_file, "%s\n", command);
            }
            else
            {
                for (int i = 0; i < no_of_commands_stored_in_history; ++i)
                {
                    fprintf(history_file, "%s\n", commands[i]);
                }
                fprintf(history_file, "%s\n", command);
            }
        }
        else if (no_of_commands_stored_in_history == 20)
        {
            for (int i = 1; i < 20; ++i)
            {
                fprintf(history_file, "%s\n", commands[i]);
            }
            fprintf(history_file, "%s\n", command);
        }

        fclose(history_file);
    }
}