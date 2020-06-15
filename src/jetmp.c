#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BUFFER_SIZE 151
#define BUFFER2_SIZE 111

char buffer[BUFFER_SIZE], buffer2[BUFFER2_SIZE], loop_count_buffer[11], initial_buffer_char;
int buffer_index = 0, buffer_length, i, j, k,
    loop_index, loop_start, loop_count,
    loop_index2, loop_start2, loop_count2,
    initial_buffer_index;
_Bool match = 0, html_escape = 1, in_loop = 0,
      in_loop2 = 0;
char **ptr_argv;
int *ptr_argc;
char prev_ch, tmp_ch;
void evaluate_char(char*, FILE*);
void buffer_key(char*, FILE*);
void print_key_value(void);
void print_partial(void);
void init_loop(_Bool*, int*, int*, int*, FILE*);
void evaluate_loop(_Bool*, int*, int*, int*, FILE*);
void init_buffer2(void);

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Usage: jetmp FILENAME KEY1:'VALUE 1' KEY2:'VALUE 2' ...\n");
    exit(EXIT_FAILURE);
  }
  FILE *file = fopen(argv[1], "r");
  if (file == NULL) {
    printf("Cannot open file %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  ptr_argv = argv;
  ptr_argc = &argc;
  char ch;
  while ((ch = getc(file)) != EOF) {
    evaluate_char(&ch, file);
  }
  fclose(file);
  return 0;
}

void evaluate_char(char *ptr_ch, FILE *file) {
  if (*ptr_ch != '{') {
    if ((in_loop && loop_count <= 0) || (in_loop2 && loop_count2 <= 0))
      return;
    putc(*ptr_ch, stdout);
    return;
  }
  *ptr_ch = getc(file);
  if (*ptr_ch != '{') {
    putc('{', stdout);
    putc(*ptr_ch, stdout);
    return;
  }
  *ptr_ch = getc(file);
  initial_buffer_char = '\0';
  initial_buffer_index = 0;
  if (*ptr_ch == '/') {
    html_escape = 0;
  } else if (*ptr_ch == '>') {
    buffer_key(ptr_ch, file);
    print_partial();
    return;
  } else if (*ptr_ch == '#') {
    buffer_key(ptr_ch, file);
    if (!in_loop) {
      init_loop(&in_loop, &loop_index, &loop_start, &loop_count, file);
    } else {
      init_buffer2();
      snprintf(buffer, BUFFER_SIZE, "%s%d", buffer2, loop_index);
      buffer_index = strlen(buffer);
      init_loop(&in_loop2, &loop_index2, &loop_start2, &loop_count2, file);
    }
    return;
  } else if (*ptr_ch != ' ') {
    initial_buffer_char = *ptr_ch;
    initial_buffer_index = 1;
  }
  buffer_key(ptr_ch, file);
  if (in_loop2)
      evaluate_loop(&in_loop2, &loop_index2, &loop_start2, &loop_count2, file);
  else if (in_loop)
      evaluate_loop(&in_loop, &loop_index, &loop_start, &loop_count, file);
  else
    print_key_value();
}

void buffer_key(char *ptr_ch, FILE *file) {
  buffer[0] = initial_buffer_char;
  buffer_index = initial_buffer_index;
  prev_ch = '\0';
  while (*ptr_ch != '}' || (*ptr_ch == '}' && prev_ch != '}')) {
    prev_ch = *ptr_ch;
    *ptr_ch = getc(file);
    if (*ptr_ch == EOF) {
      printf("JETmp syntax error: missing closing braces");
      exit(EXIT_FAILURE);
    }
    if (*ptr_ch != ' ' && *ptr_ch != '}')
      buffer[buffer_index++] = *ptr_ch;
  }
  buffer[buffer_index] = '\0';
}

void print_key_value() {
  match = 0;
  for (i = 2; i < *ptr_argc; i++) {
    for (j = 0; j < buffer_index; j++) {
      if (buffer[j] != ptr_argv[i][j]) {
        match = 0;
        break;
      } else {
        match = 1;
      }
    }
    if (!match || ptr_argv[i][j] != ':')
      continue;
    while (ptr_argv[i][++j] != '\0') {
      tmp_ch = ptr_argv[i][j];
      if (html_escape) {
        switch(tmp_ch) {
          case '&': printf("&amp;"); break;
          case '<': printf("&lt;"); break;
          case '>': printf("&gt;"); break;
          case '\"': printf("&quot;"); break;
          case '\'': printf("&#39;"); break;
          default: putc(tmp_ch, stdout);
        }
      } else {
        putc(tmp_ch, stdout);
      }
    }
    break;
  }
  html_escape = 1;
}

void print_partial() {
  FILE *file = fopen(buffer, "r");
  if (file == NULL) return;
  char ch;
  while ((ch = getc(file)) != EOF) {
    evaluate_char(&ch, file);
  }
  fclose(file);
}

void init_loop(_Bool *ptr_in_loop, int *ptr_loop_index, int *ptr_loop_start, int *ptr_loop_count, FILE *file) {
  *ptr_in_loop = 1;
  *ptr_loop_index = 1;
  *ptr_loop_start = ftell(file);
  match = 0;
  *ptr_loop_count = 0;
  loop_count_buffer[0] = '0';
  loop_count_buffer[1] = '\0';
  for (i = 2; i < *ptr_argc; i++) {
    for (j = 0; j < buffer_index; j++) {
      if (buffer[j] != ptr_argv[i][j]) {
        match = 0;
        break;
      } else {
        match = 1;
      }
    }
    if (!match || ptr_argv[i][j] != ':')
      continue;
    k = 0;
    while (ptr_argv[i][++j] != '\0') {
      loop_count_buffer[k++] = ptr_argv[i][j];
    }
  }
  *ptr_loop_count = atoi(loop_count_buffer);
}

void evaluate_loop(_Bool *ptr_in_loop, int *ptr_loop_index, int *ptr_loop_start, int *ptr_loop_count, FILE *file) {
  if (strcmp(buffer, "-") == 0) {
    if (*ptr_loop_index >= *ptr_loop_count) {
      *ptr_in_loop = 0;
      *ptr_loop_count = 0;
      *ptr_loop_index = 0;
    } else {
      *ptr_loop_index = *ptr_loop_index + 1;
      fseek(file, *ptr_loop_start, SEEK_SET);
    }
    return;
  }
  buffer_length = strlen(buffer);
  if (buffer_length >= 1 && buffer[buffer_length - 1] == '#') {
    buffer[buffer_length - 1] = '\0';
    init_buffer2();
    if (in_loop2)
      snprintf(buffer, BUFFER_SIZE, "%s%d_%d", buffer2, loop_index, *ptr_loop_index);
    else
      snprintf(buffer, BUFFER_SIZE, "%s%d", buffer2, *ptr_loop_index);
    buffer_index = strlen(buffer);
  }
  print_key_value();
}

void init_buffer2() {
  if (strlen(buffer) >= BUFFER2_SIZE) return;
  for (i = 0; i < (strlen(buffer) + 1); i++) {
    buffer2[i] = buffer[i];
  }
}
