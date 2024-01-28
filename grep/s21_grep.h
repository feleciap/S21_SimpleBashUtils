#ifndef _SRC_GREP_GREP_H_
#define _SRC_GREP_GREP_H_

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEN 1300

struct s_flag {
  int e;
  int f;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int o;
  int count_flag;
  int count_file;
  int count_template;
  char **template;
  char **name_file;
};

int flag_f(char *stroka, struct s_flag *flags);
int parsing_flag(int argc, char *argv[], struct s_flag *flags);
void parsing_file(struct s_flag flags);
int flagsrun(char *argv[], int *i, size_t j, size_t index_e,
             struct s_flag *flags, int *error);
void readline(struct s_flag *flags, int i, int *count_strok, int *count_ex_file,
              int *count_line, FILE *file);
int flags(int argc, char *argv[]);

#endif
