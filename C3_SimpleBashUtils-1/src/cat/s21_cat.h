#ifndef s21_cat_h
#define s21_cat_h
#include "getopt.h"
#include "stdio.h"
#include "stdlib.h"

struct options {
  int b;
  int n;
  int s;
  int e;
  int t;
  int v;
};

void cat(int argc, char *argv[]);
void flagi(int argc, char *argv[], struct options *opt);
void func_cat(char c, FILE *file, struct options *opt);

#endif
