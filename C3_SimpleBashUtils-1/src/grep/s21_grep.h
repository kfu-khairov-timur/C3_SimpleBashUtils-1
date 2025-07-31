#ifndef s21_grep
#define s21_grep
#define MAXLINE 4096

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct options {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int f;
  int h;
  int s;
  int o;
  int off_o;
} opt;

void File(int argc, char *argv[], char *pattern, opt *options);
void Parse(int argc, char *argv[], opt *options, char *pattern);
void FlagE(char *optarg, char *pattern, opt *options);
void FlagF(char *pattern, opt *options);
void FlagO(char *line, regex_t *regex);
int compileRegex(char *pattern, opt *options, regex_t *regex);
void FileCheck(char *filename, opt *options, regex_t *regex, int files);
int sravsstroki(regex_t *regex, opt *options, char *line);

#endif
