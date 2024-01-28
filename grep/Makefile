CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11 

all: s21_grep style

s21_grep: 
	${CC} ${FLAGS} s21_grep.c s21_grep.h -o s21_grep
	
rebuild: clean makeStyle all

clean:
	rm -f s21_grep

check:
	cppcheck --enable=all --suppress=missingIncludeSystem s21_grep.c

makeStyle:
	clang-format -i -style=Google *.c
	clang-format -i -style=Google *.h

style:
	clang-format -n -style=Google *.c
	clang-format -n -style=Google *.h