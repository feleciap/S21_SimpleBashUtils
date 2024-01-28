#include "s21_grep.h"

int main(int argc, char *argv[]) {
  if (argc > 2) {
    flags(argc, argv);
  }
  return 0;
}

int flag_f(char *stroka, struct s_flag *flags) {
  int error = 0;
  FILE *file = fopen(stroka, "rt");
  if (file == NULL) {
    fprintf(stderr, "grep: %s: No such file or directory\n", stroka);
    error = -1;
  } else {
    char line[4096] = {'\0'};
    while (fgets(line, 4096, file) != NULL) {
      line[strcspn(line, "\n")] = '\0';
      int tmp_len = strlen(line);
      int str_len = line[tmp_len];
      if (str_len == '\n') {
        str_len = tmp_len;
      } else {
        str_len = tmp_len + 1;
      }
      snprintf(flags->template[flags->count_template++], str_len, "%s", line);
    }
    fclose(file);
  }
  return error;
}

int parsing_flag(int argc, char *argv[], struct s_flag *flags) {
  size_t index_e = 1;
  int error = 0;
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      int non_SOS = 0;
      for (size_t j = 1; j < strlen(argv[i]) && non_SOS == 0; j++) {
        if (argv[i][j] == 'e') {
          flags->e = 1;
          flags->count_flag++;
          if (j == strlen(argv[i] + 1)) {
            i++;
            snprintf(flags->template[flags->count_template++], LEN, "%s",
                     argv[i]);
            non_SOS++;
          } else if (index_e != strlen(argv[i]) - 1) {
            snprintf(flags->template[flags->count_template++], LEN, "%s",
                     &argv[i][j++]);
            non_SOS++;
          }
        } else if (argv[i][0] == '-' && argv[i][1] != '-') {
          error = flagsrun(argv, &i, j, index_e, flags, &error);
        }
      }
    } else if (flags->count_template == 0) {
      snprintf(flags->template[flags->count_template++], LEN, "%s", argv[i]);
    } else {
      snprintf(flags->name_file[flags->count_file++], LEN, "%s", argv[i]);
    }
  }
  return error;
}

int flagsrun(char *argv[], int *i, size_t j, size_t index_e,
             struct s_flag *flags, int *error) {
  char simbol_error;
  *error = 0;
  if (argv[*i][j] == 'f') {
    flags->f = 1;
    flags->count_flag++;
    if (j == strlen(argv[*i] + 1)) {
      *i = *i + 1;
      *error = flag_f(argv[*i], flags);
    } else if (index_e != strlen(argv[*i]) - 1) {
      *error = flag_f(&argv[*i][j++], flags);
    }
  } else if (argv[*i][j] == 'i') {
    flags->i = 1;
    flags->count_flag++;
  } else if (argv[*i][j] == 'v') {
    flags->v = 1;
    flags->count_flag++;
  } else if (argv[*i][j] == 'c') {
    flags->c = 1;
    flags->count_flag++;
  } else if (argv[*i][j] == 'l') {
    flags->l = 1;
    flags->count_flag++;
  } else if (argv[*i][j] == 'n') {
    flags->n = 1;
    flags->count_flag++;
  } else if (argv[*i][j] == 'h') {
    flags->h = 1;
    flags->count_flag++;
  } else if (argv[*i][j] == 's') {
    flags->s = 1;
    flags->count_flag++;
  } else if (argv[*i][j] == 'o') {
    flags->o = 1;
    flags->count_flag++;
  } else {
    simbol_error = argv[*i][j];
    printf("grep: invalid option -- %c\n usage: grep [-eivclnhs]\n",
           simbol_error);
    flags->count_flag = -1;
    *error = -1;
  }
  return *error;
}

void parsing_file(struct s_flag flags) {
  for (int i = 0; i < flags.count_file; ++i) {
    FILE *file = fopen(flags.name_file[i], "rt");
    if (file == NULL) {
      if (!flags.s) {
        fprintf(stderr, "grep: %s: No such file or directory\n",
                flags.name_file[i]);
      }
    } else {
      int count_strok = 0, count_ex_file = 0, count_line = 1;
      readline(&flags, i, &count_strok, &count_ex_file, &count_line, file);
      if (flags.c) {
        if (flags.count_file > 1 && !flags.h) {
          printf("%s:", flags.name_file[i]);
        }
        if (flags.l && count_strok) {
          count_strok = 1;
        }
        printf("%d\n", count_strok);
        count_strok = 0;
      }
      if (flags.l && count_ex_file == 1) {
        printf("%s\n", flags.name_file[i]);
        count_ex_file = 0;
      }

      fclose(file);
    }
  }
}

void readline(struct s_flag *flags, int i, int *count_strok, int *count_ex_file,
              int *count_line, FILE *file) {
  char line[4096] = {'\0'};
  int reg_match = 1;
  while ((fgets(line, 4096, file)) != NULL) {
    char *pattern = 0;
    for (int j = 0; j < flags->count_template; j++) {
      regex_t preg;
      int reg_option = flags->i ? REG_ICASE : REG_EXTENDED;
      regcomp(&preg, flags->template[j], reg_option);
      reg_match = regexec(&preg, line, 0, 0, 0);
      pattern = flags->template[j];
      if (reg_match == 0) {
        j = flags->count_template;
      }
      regfree(&preg);
    }
    if (flags->v) {
      if (reg_match)
        reg_match = 0;
      else
        reg_match = 1;
    }
    if (!reg_match) {
      *count_strok = *count_strok + 1;
      *count_ex_file = 1;
    }
    if (!reg_match && !flags->l && !flags->c) {
      if (flags->count_file > 1 && !flags->h) {
        printf("%s:", flags->name_file[i]);
      }
      if (flags->n) {
        printf("%d:", *count_line);
      }
      if (flags->o && reg_match == 0) {
        for (int k = 0; k < (int)strlen(line); k++) {
          if (line[k] == *pattern) {
            char *new_pattern = pattern;
            int flag1 = 0;
            for (int h = 0; h < (int)strlen(new_pattern); h++) {
              if (line[k + h] != *new_pattern) flag1 = 1;
              new_pattern++;
            }
            if (flag1 == 0) printf("%s\n", pattern);
          }
        }
      } else {
        printf("%s", line);
        if (line[strlen(line) - 1] != '\n') {
          printf("\n");
        }
      }
    }
    *count_line = *count_line + 1;
  }
}

int flags(int argc, char *argv[]) {
  struct s_flag flags;
  flags.e = 0, flags.f = 0, flags.i = 0, flags.v = 0, flags.c = 0, flags.l = 0,
  flags.n = 0, flags.h = 0, flags.s = 0, flags.o = 0, flags.count_flag = 0,
  flags.count_file = 0, flags.count_template = 0;

  flags.template = (char **)malloc(LEN * sizeof(char *));
  flags.name_file = (char **)malloc(LEN * sizeof(char *));
  for (int i = 0; i < LEN; i++) {
    flags.template[i] = (char *)malloc(LEN * sizeof(char));
    flags.name_file[i] = (char *)malloc(LEN * sizeof(char));
  }
  int error_flag = parsing_flag(argc, argv, &flags);
  if (!error_flag) {
    parsing_file(flags);
  }
  for (int i = 0; i < LEN; i++) {
    free(flags.template[i]);
    free(flags.name_file[i]);
  }
  free(flags.template);
  free(flags.name_file);
  return 0;
}
