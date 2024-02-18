#ifndef SRC_S21_GREP_H
#define SRC_S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct flag_option {
  short e;
  short i;
  short v;
  short c;
  short l;
  short n;
  short h;
  short s;
  short f;
  short o;
  char** template;
  int count_temp;
  int line_num;
  int line_count;
  short file_check;
  char* name_file;
  short check_l;
} flag;

int option_index;
static struct option long_options[] = {{0, 0, 0, 0}};

void s21_grep(flag* tab, char* line);
void open_file(int argc, char** argv, flag* tab);
void parser(int argc, char** argv, flag* tab);
void get_template(char* template, flag* tab);
int comp_template(flag* tab, char* line, short num_temp);
void print_result(flag* tab, char* line);
void flag_f(char* path, flag* tab);

#endif  // SRC_S21_GREP_H