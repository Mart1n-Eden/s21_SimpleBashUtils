#ifndef SRC_S21_Cat_H
#define SRC_S21_Cat_H

#include <dirent.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int option_index;
static struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                       {"squeeze-blank", 0, 0, 's'},
                                       {"number", 0, 0, 'n'},
                                       {0, 0, 0, 0}};

typedef struct flag_option {
  short b;
  short e;
  short n;
  short s;
  short t;
  short v;
  short T;
  short E;
} flag;

int parser(int argc, char** argv, flag* tab);
void s21_cat(flag* tab, FILE* file);
void open_file(char* path, flag* tab);
void flag_b(FILE* file);
void flag_n(FILE* file);
void flag_s(FILE* file);
void flag_E(FILE* file);
void flag_T(FILE* file);
void flag_v(FILE* file, short f);
void no_flag(FILE* file);

#endif  // SRC_S21_Cat_H