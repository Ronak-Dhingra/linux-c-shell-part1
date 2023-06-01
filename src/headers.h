#ifndef _HEADERS_H
#define _HEADERS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/utsname.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <libgen.h>
#include <assert.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define AC_BLUE "\x1b[34m"
#define AC_WHITE "\x1b[37m"
#define AC_GREEN "\x1b[32m"

#define clear() printf("\033[H\033[J")
#define SYS_NAME_LEN 100
#define CURR_DIR_LEN 100
#define MAX_COMMAND_LEN 100
#define MAX_NO_COMMANDS_IN_INPUT 100
#define HIS_STORAGE 100

#endif