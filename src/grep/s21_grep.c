#define _POSIX_C_SOURCE 200809L
#include "s21_grep.h"

int main(int argc, const char **const argv) {
  char *filenames[BUFF_SIZE] = {NULL};
  char *patterns[BUFF_SIZE] = {NULL};
  int flag['z'] = {0};

  int err_code = argc >= 3 ? parse_args(flag, argv, filenames, patterns, argc)
                           : NON_EXISTENT_FLAG;

  if (err_code == SUCCESS) {
    bool is_no_ef = false;
    if (*patterns == NULL) {
      is_no_ef = true;
      *patterns = malloc(sizeof(char) * strlen(*filenames) + 1);
      if (*patterns == NULL)
        err_code = MALLOC_ERROR;
      else
        strcpy(*patterns, *filenames);
    }
    int file_count = get_file_count(filenames) - is_no_ef;
    for (int i = is_no_ef; filenames[i] && err_code == SUCCESS; i++)
      grep(flag, filenames[i], patterns, file_count);
  }
  for (int i = 0; filenames[i]; i++) free(filenames[i]);
  for (int i = 0; patterns[i]; i++) free(patterns[i]);
  print_err(err_code);
  return err_code;
}

void grep(const int *const flags, const char *const filename, char *patterns[],
          const int file_count) {
  FILE *file = fopen(filename, "r");
  if (file) {
    char *line = NULL;
    size_t linecap = 0;
    int matched_lines = 0;
    int line_num = 0;
    char *matches[BUFF_SIZE] = {NULL};
    while (getline(&line, &linecap, file) > 0) {
      line_num++;
      if (match_patterns(flags, line, patterns, matches)) {
        matched_lines++;
        if (!flags['c'] && !flags['l']) {
          handle_header(flags, filename, file_count);
          handle_number(flags, line_num);
          if (flags['o'] && !flags['v']) {
            for (int j = 0; matches[j]; j++) printf("%s\n", matches[j]);
          } else {
            line[strlen(line) - 1] == '\n' ? printf("%s", line)
                                           : printf("%s\n", line);
          }
        }
      }
      for (int j = 0; matches[j]; j++) free(matches[j]);
    }
    handle_header_cl(flags, filename, file_count);
    handle_count(flags, matched_lines);
    handle_list_files(flags, filename, matched_lines);
    free(line);
    fclose(file);
  } else if (!flags['s']) {
    print_no_file_err(filename);
  }
}
bool match_patterns(const int *const flags, const char *const str,
                    char *patterns[], char *matches[]) {
  int idx = 0;
  for (int i = 0; patterns[i]; i++) {
    regex_t regex;
    if (flags['i'])
      regcomp(&regex, patterns[i], REG_ICASE);
    else
      regcomp(&regex, patterns[i], REG_EXTENDED);

    regmatch_t match;
    size_t offset = 0;
    size_t str_len = strlen(str);

    for (int ret; (ret = regexec(&regex, str + offset, 1, &match, 0)) == 0;
         idx++) {
      int len = match.rm_eo - match.rm_so;
      matches[idx] = malloc(len + 1);
      memcpy(matches[idx], str + match.rm_so + offset, len);
      matches[idx][len] = '\0';
      offset += match.rm_eo;
      if (offset > str_len) break;
    }
    matches[idx] = NULL;
    regfree(&regex);
  }
  return flags['v'] ? !idx : idx;
}
void handle_header(const int *const flags, const char *const filename,
                   const int file_count) {
  if (file_count > 1 && !flags['c'] && !flags['h']) printf("%s:", filename);
}
void handle_number(const int *const flags, const int line_num) {
  if (flags['n']) printf("%d:", line_num);
}
void handle_header_cl(const int *const flags, const char *const filename,
                      const int file_count) {
  if (flags['c'] && !flags['h'] && file_count > 1) printf("%s:", filename);
}

void handle_count(const int *const flags, const int matched_lines) {
  if (flags['c']) {
    printf("%d\n", flags['l'] ? matched_lines > 0 : matched_lines);
  }
}
void handle_list_files(const int *const flags, const char *const filename,
                       const int matched_lines) {
  if (flags['l'] && matched_lines) printf("%s\n", filename);
}
int parse_args(int *const flags, const char **const argv, char *filenames[],
               char *patterns[], const int argc) {
  int err = SUCCESS;
  int p = 0;
  int f = 0;
  for (int i = 1; i < argc; i++) {
    if (parse_flags(flags, argv[i])) {
      if (check_flags(argv[i])) {
        err = NON_EXISTENT_FLAG;
        break;
      }
      if ((flags['e'] || flags['f']) && !argv[i]) {
        err = NON_EXISTENT_FLAG;
        break;
      }
      if (flags['e']) {
        i++;
        patterns[p] = malloc(sizeof(char) * (strlen(argv[i]) + 1));
        if (patterns[p] == NULL) {
          err = MALLOC_ERROR;
          break;
        }
        strcpy(patterns[p], argv[i]);
        p++;
      } else if (flags['f']) {
        i++;
        if ((err = get_patterns_from_file(argv[i], patterns, &p)) != SUCCESS) {
          break;
          ;
        }
      }
      reset_e_f(flags);
    } else {
      filenames[f] = malloc(sizeof(char) * (strlen(argv[i]) + 1));
      if (filenames[f] == NULL) {
        err = MALLOC_ERROR;
        break;
      }
      strcpy(filenames[f], argv[i]);
      f++;
    }
  }
  patterns[p] = NULL;
  filenames[f] = NULL;
  return err;
}
int get_patterns_from_file(const char *const filename, char *patterns[],
                           int *idx) {
  int err = SUCCESS;
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    print_no_file_err(filename);
    err = NO_FILE;
  } else {
    char *pat = NULL;
    size_t linecap = 0;
    ssize_t linelen = 0;
    while ((linelen = getline(&pat, &linecap, file)) > 0) {
      patterns[*idx] = malloc(sizeof(char) * (linelen + 1));
      if (patterns[*idx] == NULL) {
        err = MALLOC_ERROR;
        break;
      }
      strcpy(patterns[*idx], pat);
      patterns[*idx][strcspn(patterns[*idx], "\r\n")] = '\0';
      if (!strlen(patterns[*idx])) {
        patterns[*idx][0] = '.';
      }
      (*idx)++;
    }
    fclose(file);
    free(pat);
  }
  return err;
}

bool parse_flags(int *const flags, const char *const str) {
  bool is_set = false;
  if (str[0] == '-') {
    for (int i = 1; str[i]; i++) flags[(int)str[i]] = 1;
    is_set = true;
  }
  return is_set;
}

void print_usage_err() {
  printf(
      "usage: grep [-ivclnhso] [-e pattern] [-f file] template "
      "[file ...]\n");
}

void print_no_file_err(const char *const filename) {
  printf("grep: %s: No such file or directory\n", filename);
}

void reset_e_f(int *const flags) {
  flags['e'] = 0;
  flags['f'] = 0;
}

int get_file_count(char *const filenames[]) {
  int count = 0;
  while (*filenames++) count++;
  return count;
}
void print_err(int err_code) {
  if (err_code == NON_EXISTENT_FLAG) print_usage_err();
  if (err_code == MALLOC_ERROR) print_malloc_err();
}
void print_malloc_err() {
  printf("grep: [MALLOC]: Can't allocate enough memory!:(\n");
}

bool check_flags(const char *const argv) {
  int validation[BUFF_SIZE] = {0};
  validation['i'] = 1;
  validation['v'] = 1;
  validation['c'] = 1;
  validation['l'] = 1;
  validation['n'] = 1;
  validation['h'] = 1;
  validation['s'] = 1;
  validation['o'] = 1;
  validation['f'] = 1;
  validation['e'] = 1;
  int res = false;
  for (int i = 1; argv[i]; i++) {
    if (!validation[(int)argv[i]]) {
      res = true;
      break;
    }
  }
  return res;
}
