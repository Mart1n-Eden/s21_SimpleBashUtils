CC=gcc

CFLAGS=-Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: s21_grep

s21_grep:
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

valgrind: rebuild
	bash test_leaks.sh

clean:
	rm -rf s21_grep

rebuild: clean s21_grep
