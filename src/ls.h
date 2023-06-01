#ifndef _LS_H
#define _LS_H

#define MAX_NO_COMMANDS_IN_INPUT 100
int alphasort(const struct dirent **a, const struct dirent **b);
int checkifFileis_executable(const char *filename);
int is_reg_file(const char *path);
int is_dir(const char *path);
void ls_print(struct dirent *dp);
void file_status2(const char *filepath);
void file_status(struct dirent *dir);
void ls_implementation(char command_words[MAX_NO_COMMANDS_IN_INPUT][100], int len, char curr_dir[100], int number_of_commands);

#endif