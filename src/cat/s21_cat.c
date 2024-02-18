#include "s21_cat.h"

int main(int argc, char** argv) {
  flag tab = {0};
  short count_file;
  count_file = parser(argc, argv, &tab);
  for (int i = count_file; i < argc; i++) {
    open_file(argv[i], &tab);
  }
}

int parser(int argc, char** argv, flag* tab) {
  short opt = 0;
  while ((opt = getopt_long(argc, argv, "+benstvTE", long_options,
                            &option_index)) != -1) {
    switch (opt) {
      case 'b':
        tab->b++;
        break;
      case 'e':
        tab->e++;
        tab->v++;
        break;
      case 'n':
        tab->n++;
        break;
      case 's':
        tab->s++;
        break;
      case 't':
        tab->t++;
        tab->v++;
        break;
      case 'v':
        tab->v++;
        break;
      case 'T':
        tab->t++;
        break;
      case 'E':
        tab->e++;
        break;
      default:
        fprintf(stderr,
                "cat: illegal option -- %c\nusage: cat [-benstvET] [file ...]",
                opt);
        break;
    }
  }
  return optind;
}

void open_file(char* path, flag* tab) {
  FILE* file = fopen(path, "r");
  if (file) {
    s21_cat(tab, file);
    fclose(file);
  } else {
    fprintf(stderr, "cat: %s: No such file or directory\n", path);
  }
}

void s21_cat(flag* tab, FILE* file) {
  int ch, prev_ch = '\n';
  short nl = 1, lc = 1;
  while ((ch = fgetc(file)) != EOF) {
    if (tab->s) {
      if (nl > 1 && ch == '\n') {
        continue;
      } else {
        if (ch == '\n') {
          nl++;
        } else {
          nl = 0;
        }
      }
    } else {
      if (ch == '\n') {
        nl++;
      } else {
        nl = 0;
      }
    }
    if (!tab->b && tab->n) {
      if (prev_ch == '\n') {
        printf("%6d\t", lc);
        lc++;
      }
    } else if (tab->b) {
      if (prev_ch == '\n' && ch != '\n') {
        printf("%6d\t", lc);
        lc++;
      }
    }
    prev_ch = ch;
    if (tab->t && ch == '\t') {
      printf("^I");
      continue;
    }
    if (tab->e && ch == '\n') {
      printf("$\n");
      continue;
    }
    if (tab->v) {
      if ((ch >= 0 && ch < 9) || (ch > 10 && ch <= 31)) {
        printf("^%c", ch + 64);
      } else if (ch == 127) {
        printf("^?");
      } else if (ch > 127 && ch < 160) {
        printf("M-^%c", (ch & 31) + 64);
      } else {
        printf("%c", ch);
      }
    } else {
      printf("%c", ch);
    }
  }
}
