all: clang s21_cat test
s21_cat:
	rm -rf s21_cat
	gcc -Wall -Werror -Wextra s21_cat.c -o s21_cat

clang:
	cp ../../materials/linters/.clang-format ./
	clang-format -i *.c *.h
	rm .clang-format
test:
	sh test_func_cat.sh