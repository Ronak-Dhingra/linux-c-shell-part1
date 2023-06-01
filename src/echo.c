#include "headers.h"
#include "echo.h"

void echo_implementation(char command_words[MAX_NO_COMMANDS_IN_INPUT][100], int number_of_commands)
{
    for (int i = 1; i < number_of_commands; ++i)
    {
        printf("\033[0;37m%s ", command_words[i]);
    }
    printf("\n");
}