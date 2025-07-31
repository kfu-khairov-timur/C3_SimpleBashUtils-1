#include "s21_cat.h"

int main(int argc, char *argv[]) {
  cat(argc, argv);
  return 0;
}

void cat(int argc, char *argv[]) {
  struct options opt = {0};
  flagi(argc, argv, &opt);
  if (opt.b == 1) {
    opt.n = 0;
  }
  int i = optind;
  while (i < argc) {
    FILE *file = fopen(argv[i], "r");
    if (file) {
      char c = '\0';
      func_cat(c, file, &opt);
      fclose(file);
    } else {
      fprintf(stderr, "File '%s' does not exist.\n", argv[i]);
    }
    i++;
  }
}

void flagi(int argc, char *argv[], struct options *opt) {
  int option;
  struct option long_opt[] = {{"number-nonblank", no_argument, 0, 'b'},
                              {"number", no_argument, 0, 'n'},
                              {"squeeze-blank", no_argument, 0, 's'},
                              {0, 0, 0, 0}};
  int option_index = 0;

  while ((option = getopt_long(argc, argv, "+benstvET", long_opt,
                               &option_index)) != -1) {
    if (option == 'b') {
      opt->n = 0;
      opt->b = 1;
    } else if (option == 'n') {
      opt->n = 1;
    } else if (option == 's') {
      opt->s = 1;
    } else if (option == 'e') {
      opt->e = 1;
      opt->v = 1;
    } else if (option == 'E') {
      opt->e = 1;
    } else if (option == 'T' || option == 't') {
      opt->t = 1;
      if (option == 't') {
        opt->v = 1;
      }
    } else if (option == 'v') {
      opt->v = 1;
    }
  }
}

void func_cat(char c, FILE *file, struct options *opt) {
  int point = 0;
  int counter = 0;
  char prev_c = '\n';
  int line_number = 1;
  while ((c = fgetc(file)) != EOF) {
    if (opt->s) {
      if (prev_c == '\n' && (c == '\n')) {
        if (counter > 0) {
          point = 1;
        }
        counter++;
      } else {
        counter = 0;
      }
    }
    if (opt->n && counter <= 1) {
      if (prev_c == '\n') {
        printf("%6d\t", line_number);
        line_number++;
      }
    }
    if (opt->b && prev_c == '\n' && c != '\n') {
      printf("%6d\t", line_number);
      line_number++;
    }
    if (opt->e && counter <= 1 && c == '\n') {
      printf("$");
    }
    if (opt->t && c == '\t') {
      point = 1;
      printf("^I");
    }
    if (opt->v && ((c != '\t' && c != '\n' && c < 32) || c == 127)) {
      point = 1;
      if (c == 127) {
        printf("^?");
      } else {
        printf("^%c", c + 64);
      }
    }
    if (!point) {
      putchar(c);
    }
    point = 0;
    prev_c = c;
  }
}
