CC=gcc

CFLAGS=-Wall -Werror -Wextra -std=c11 -D_GNU_SOURCE

all: s21_cat

s21_cat:
	$(CC) $(CFLAGS) s21_cat.c -o s21_cat

valgrind: rebuild
	bash test_leaks.sh

clean:
	rm -rf s21_cat

rebuild: clean s21_cat
