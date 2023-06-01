#ifndef _HISTORY_
#define _HISTORY_

int getHistory(char curr_dir[100], char *commands[100]);
void history_implementation(char curr_dir[100]);
void add_to_History(char command[100], char curr_dir[100], int len);

#endif