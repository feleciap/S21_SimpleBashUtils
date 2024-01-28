#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_

#include <stdio.h>
#include <string.h>

struct short_flag;
void openFile(char *file_name, struct short_flag options);
int parsingFlag(int argc, char *argv[], struct short_flag *options);
void parsingFile(int argc, char *argv[], struct short_flag options);
void endfile(char *file_name, struct short_flag options);
int flags(char *argv[], struct short_flag *options, int i, int count_flag,
          char simbol_error);

#endif
