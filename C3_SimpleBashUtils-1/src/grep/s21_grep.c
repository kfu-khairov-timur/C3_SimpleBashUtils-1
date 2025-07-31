#include "s21_grep.h"

int main(int argc, char *argv[]) {
  char pattern[4096] = {0};
  opt options = {0};
  Parse(argc, argv, &options, pattern);
  File(argc, argv, pattern, &options);
  return 0;
}

void Parse(int argc, char *argv[], opt *options, char *pattern) {
  int opt;
  while ((opt = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, 0)) != -1) {
    if (opt == 'e') {
      options->e = 1;
      FlagE(optarg, pattern, options);
    } else if (opt == 'i') {
      options->i = 1;
    } else if (opt == 'v') {
      options->v = 1;
    } else if (opt == 'c') {
      options->c = 1;
    } else if (opt == 'l') {
      options->l = 1;
    } else if (opt == 'n') {
      options->n = 1;
    } else if (opt == 'h') {
      options->h = 1;
    } else if (opt == 's') {
      options->s = 1;
    } else if (opt == 'f') {
      options->f = 1;
      FlagF(pattern, options);
    } else if (opt == 'o') {
      options->o = 1;
    } else {
      fprintf(stderr, "usage");
      exit(1);
    }
  }
  if (options->off_o == 1) options->o = 0;
}
void File(int argc, char *argv[], char *pattern, opt *options) {
  if (argc < 3 || optind == argc) {
    fprintf(stderr, "Usage: %s pattern file\n", argv[0]);
  } else {
    int current_file = 0;
    int files = 0;
    if (!options->e && !options->f) {
      current_file = optind + 1;
      strcat(pattern, argv[optind]);
    } else {
      current_file = optind;
    }
    if (argc - current_file != 1) {
      files = 1;
    } else {
      files = 0;
    }
    regex_t regex = {0};
    if (compileRegex(pattern, options, &regex)) {
      for (int i = current_file; i < argc; i++) {
        FileCheck(argv[i], options, &regex, files);
      }
    }
    regfree(&regex);
  }
}

int compileRegex(char *pattern, opt *options, regex_t *regex) {
  int rgcmp = 1;
  if (options->i) {
    if (regcomp(regex, pattern, REG_EXTENDED | REG_ICASE | REG_NEWLINE) != 0) {
      fprintf(stderr, "Failed to compile regular expression\n");
      rgcmp = 0;
    }
  } else if (regcomp(regex, pattern, REG_EXTENDED | REG_NEWLINE) != 0) {
    fprintf(stderr, "Failed to compile regular expression\n");
    rgcmp = 0;
  }
  return rgcmp;
}

void FileCheck(char *filename, opt *options, regex_t *regex, int files) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    if (!options->s) {
      fprintf(stderr, "Unable to open file: %s\n", filename);
    }
  } else {
    char line[MAXLINE];
    int line_num = 0;
    int sovpad_line = 0;
    while (fgets(line, MAXLINE, file) != NULL) {
      line_num++;
      if (sravsstroki(regex, options, line)) {
        if (options->c || options->l) {
          sovpad_line++;
        } else {
          if (files && (!options->h)) {
            printf("%s:", filename);
          }
          if (options->n) printf("%d:", line_num);
          if (options->o && !options->v) {
            FlagO(line, regex);
          } else {
            printf("%s", line);
            if (strchr(line, '\n') == NULL) printf("\n");
          }
        }
      }
    }
    if (options->c || options->l) {
      if (files && (!options->h) && options->c) {
        printf("%s:", filename);
      }
    }
    if (options->c) {
      if (options->l && sovpad_line > 0) sovpad_line = 1;
      printf("%d\n", sovpad_line);
    }
    if (options->l && sovpad_line != 0) {
      printf("%s\n", filename);
    }
    fclose(file);
  }
}

int sravsstroki(regex_t *regex, opt *options, char *line) {
  int str_match = 0;
  int regexec_result = regexec(regex, line, 0, NULL, 0);
  if ((regexec_result == 0 && !options->v) ||
      (regexec_result != 0 && options->v)) {
    str_match = 1;
  }
  return str_match;
}

void FlagE(char *optarg, char *pattern, opt *options) {
  if (options->e) {
    if (strlen(pattern) > 0) strcat(pattern, "|");
    strcat(pattern, optarg);
  }
}

void FlagF(char *pattern, opt *options) {
  FILE *f = fopen(optarg, "r");
  if (f == NULL) {
    fprintf(stderr, "Unable to open file: %s\n", optarg);
  } else {
    char buffer[MAXLINE];
    size_t pattern_len = strlen(pattern);
    size_t offset = pattern_len;

    while (fgets(buffer, MAXLINE - 1, f) != NULL) {
      if (buffer[strlen(buffer) - 1] == '\n') {
        buffer[strlen(buffer) - 1] = '\0';
      }
      if (buffer[0] != '\0') {
        if (pattern_len > 0) {
          pattern[offset++] = '|';
        }
        size_t buffer_len = strlen(buffer);
        for (size_t i = 0; i < buffer_len; i++) {
          pattern[offset++] = buffer[i];
        }
      }
      if (buffer[0] == '\0') {
        if (pattern_len > 0) {
          pattern[offset++] = '|';
        }
        pattern[offset++] = '\n';
        pattern[offset++] = '|';
        pattern[offset++] = '.';
        options->off_o = 1;
      }
    }
    fclose(f);
  }
}

void FlagO(char *line, regex_t *regex) {
  regmatch_t match;
  char result[MAXLINE];
  int result_index = 0;

  while (regexec(regex, line, 1, &match, 0) == 0) {
    int length = match.rm_eo - match.rm_so;
    for (int i = 0; i < length; i++) {
      result[result_index++] = line[match.rm_so + i];
    }
    result[result_index++] = '\n';
    line += match.rm_eo;
  }
  result[result_index] = '\0';
  printf("%s", result);
}
