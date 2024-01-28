CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11 

all: s21_cat style

s21_cat: 
	$(CC) $(FLAGS) s21_cat.c s21_cat.h -o s21_cat

rebuild: clean makeStyle all 

clean:
	rm -f s21_cat

check:
	cppcheck --enable=all --suppress=missingIncludeSystem s21_cat.c

makeStyle:
	clang-format -i -style=Google *.c
	clang-format -i -style=Google *.h

style:
	clang-format -n -style=Google *.c
	clang-format -n -style=Google *.h
