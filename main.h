#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <time.h>

#define PATH_SIZE 270

FILE *_popen(const char *cmdstring, const char *type);
int _pclose(FILE * fp);
