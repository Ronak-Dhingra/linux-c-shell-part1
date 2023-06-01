#ifndef _DISCOVER_H
#define _DISCOVER_H

#define MAX_NO_COMMANDS_IN_INPUT 100
void find_all_files(char directory[1000], int d_flag, int f_flag, int len);
void search_for_file(char file[100], char directory[1000], int len);
void discover(char command_words[MAX_NO_COMMANDS_IN_INPUT][100], int len, char curr_dir[100], int no_of_commands);


#endif