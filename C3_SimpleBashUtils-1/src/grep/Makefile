all: clang s21_grep test
s21_grep:
	rm -rf s21_grep
	gcc -Wall -Werror -Wextra s21_grep.c -o s21_grep

test:
	sh test_func_grep.sh
	
clang:
	cp ../../materials/linters/.clang-format ./
	clang-format -i *.c *.h
	rm .clang-format
