#include "s21_grep.h"

int main(int argc, char** argv) {
  flag tab = {0};
  if (argc > 2) {
    parser(argc, argv, &tab);
    open_file(argc, argv, &tab);
  } else {
    fprintf(stderr, "usage: s21_grep [pattern] [file] [flags]\n");
  }
  if (tab.f || tab.e)
    for (int i = 0; i < tab.count_temp; i++) free(tab.template[i]);
  free(tab.template);
  // free(&tab);
}

void parser(int argc, char** argv, flag* tab) {
  int opt;
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", long_options,
                            &option_index)) != -1) {
    switch (opt) {
      case 'e':
        tab->e = 1;
        get_template(optarg, tab);
        break;
      case 'i':
        tab->i = 1;
        break;
      case 'v':
        tab->v = 1;
        break;
      case 'c':
        tab->c = 1;
        break;
      case 'l':
        tab->l = 1;
        break;
      case 'n':
        tab->n = 1;
        break;
      case 'h':
        tab->h = 1;
        break;
      case 's':
        tab->s = 1;
        break;
      case 'f':
        tab->f = 1;
        flag_f(optarg, tab);
        break;
      case 'o':
        tab->o = 1;
        break;
      default:
        break;
    }
  }
  if (!tab->e && !tab->f) {
    get_template(argv[optind], tab);
    optind++;
  }
  if (optind < argc - 1) {
    tab->file_check = 1;
  } else {
    tab->file_check = 0;
  }
}

void get_template(char* template, flag* tab) {
  tab->template =
      realloc(tab->template, (tab->count_temp + 1) * sizeof(tab->template));
  tab->template[tab->count_temp] =
      (char*)calloc(strlen(template) + 1, sizeof(char));
  strcpy(tab->template[tab->count_temp], template);
  tab->count_temp++;
  tab->e = 1;
}

void flag_f(char* path, flag* tab) {
  FILE* file = fopen(path, "r");
  char* line = NULL;
  size_t len = 0;
  if (file) {
    while ((getline(&line, &len, file)) != EOF) {
      if (line != NULL) {
        tab->template = realloc(tab->template,
                                (tab->count_temp + 1) * sizeof(tab->template));
        tab->template[tab->count_temp] =
            (char*)calloc(strlen(line) + 1, sizeof(char));
        if (strlen(line) > 1 && line[strlen(line) - 1] == '\n') {
          strncpy(tab->template[tab->count_temp], line, strlen(line) - 1);
        } else {
          strncpy(tab->template[tab->count_temp], line, strlen(line));
        }
        tab->count_temp++;
      }
    }
    free(line);
    fclose(file);
  } else {
    fprintf(stderr, "after flag 'f', %s - isn't file\n", path);
  }
}

void open_file(int argc, char** argv, flag* tab) {
  if (optind >= argc && !tab->s) {
    fprintf(stderr, "NO FILES to s21_grep\n");
  } else {
    for (int i = optind; i < argc; i++) {
      FILE* file = fopen(argv[i], "r");
      char* line = NULL;
      size_t len = 0;
      if (file) {
        tab->name_file = argv[i];
        tab->line_count = 0;
        while ((getline(&line, &len, file)) != EOF) {
          if (!tab->check_l) {
            tab->line_num++;
            s21_grep(tab, line);
          }
        }
        free(line);
        if (tab->c) {
          if (tab->file_check) {
            !tab->h ? printf("%s:", tab->name_file) : 0;
            if (tab->l) {
              tab->line_count ? printf("1\n%s\n", tab->name_file)
                              : printf("0\n");
            } else {
              printf("%d\n", tab->line_count);
            }
          } else {
            if (tab->l)
              tab->line_count ? printf("1\n%s\n", tab->name_file)
                              : printf("0\n");
            else
              printf("%d\n", tab->line_count);
          }
        }
        fclose(file);
      } else {
        fprintf(stderr, "grep: %s: No such file or directory\n", argv[i]);
      }
      tab->line_num = 0;
      tab->check_l = 0;
    }
  }
}

void s21_grep(flag* tab, char* line) {
  int check = 0;
  for (int i = 0; i < tab->count_temp; i++) {
    int err;
    err = comp_template(tab, line, i);
    if (!err) {
      check = 1;
    }
  }
  if (tab->v) {
    if (!check) {
      tab->line_count++;
      print_result(tab, line);
    }
  } else {
    if (check) {
      tab->line_count++;
      print_result(tab, line);
    }
  }
}

void print_result(flag* tab, char* line) {
  if (!tab->c) {
    if (!tab->l) {
      if (tab->file_check) {
        !tab->h ? printf("%s:", tab->name_file) : 0;
        tab->n ? printf("%d:%s", tab->line_num, line) : printf("%s", line);
      } else {
        tab->n ? printf("%d:%s", tab->line_num, line) : printf("%s", line);
      }
      if (line[strlen(line) - 1] != '\n') printf("\n");
    } else {
      tab->line_count ? printf("%s\n", tab->name_file) : 0;
      tab->check_l = 1;
    }
  }
}

int comp_template(flag* tab, char* line, short num_temp) {
  int err = 0, f;
  regex_t reg;
  if (tab->i)
    f = REG_ICASE | REG_EXTENDED | REG_NEWLINE;
  else
    f = REG_EXTENDED | REG_NEWLINE;
  regcomp(&reg, tab->template[num_temp], f);
  err = regexec(&reg, line, 0, 0, 0);
  regfree(&reg);
  return err;
}