#include "./json_parser.h"
#include "./jetmp_definitions.h"

int main(int argc, char *argv[]) {
  int key_length = 51;
  int value_length = 1001;
  if (argc < 3) exit_with_usage();
  if (argc >= 4) {
    int i;
    for (i = 3; i < argc; i++) {
      if ((strcmp(argv[i], "--escape-html") == 0)) {
        escape_html = 1;
      } else if ((strcmp(argv[i], "--keys") == 0)) {
        if (argc == i + 1) exit_with_usage();
        max_keys = atoi(argv[i + 1]);
      } else if ((strcmp(argv[i], "--key-length") == 0)) {
        if (argc == i + 1) exit_with_usage();
        key_length = atoi(argv[i + 1]) + 1;
      } else if ((strcmp(argv[i], "--value-length") == 0)) {
        if (argc == i + 1) exit_with_usage();
        value_length = atoi(argv[i + 1]) + 1;
      }
    }
  }
  char keys_array[max_keys][key_length];
  char values_array[max_keys][value_length];
  int word_length = value_length > 201 ? value_length : 201;
  char word[word_length];
  file = fopen(argv[1], "r");
  if (file == NULL) {
    printf("Cannot open %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  json_to_array(argv[2], key_length, keys_array, value_length, values_array, word);
  interpolate_string(key_length, keys_array, value_length, values_array, word);
}

void exit_with_usage() {
  printf("Usage: jetmp FILENAME JSON [--escape-html] [--keys INT] [--key-length INT] [--value-length INT]\n");
  exit(EXIT_FAILURE);
}


void interpolate_string(int k, char keys_array[][k], int v, char values_array[][v], char *word) {
  while ((ch = getc(file)) != EOF) {
    validate_character(k, keys_array, v, values_array, word, -1);
  }
  fclose(file);
}

void validate_character(int k, char keys_array[][k], int v, char values_array[][v], char *word, int loop_index) {
  if (searching_for_start) {
    validate_character_for_start();
  } else {
    validate_character_for_end(k, keys_array, v, values_array, word, loop_index);
  }
}

void validate_character_for_start() {
  if (!previous_char_bracket && ch == '{') {
    previous_char_bracket = 1;
  } else if (previous_char_bracket) {
    validate_char_for_prev_bracket();
  } else {
    putc(ch, stdout);
  }
}

void validate_char_for_prev_bracket() {
  if (ch != '{') {
    previous_char_bracket = 0;
    putc('{', stdout);
    putc(ch, stdout);
  } else if (ch == '{') {
    searching_for_start = 0;
    previous_double_bracket = 1;
  }
}

void validate_character_for_end(int k, char keys_array[][k], int v, char values_array[][v], char *word, int loop_index) {
  if (previous_double_bracket && ch == '>') {
    rendering_partial = 1;
  } else if (previous_double_bracket && ch == '+') {
    getting_loop_tag = 1;
  } else if (!previous_char_bracket && ch == '}') {
    previous_char_bracket = 1;
    previous_double_bracket = 0;
  } else if (previous_char_bracket) {
    previous_double_bracket = 0;
    validate_char_for_prev_bracket_end(k, keys_array, v, values_array, word, loop_index);
  } else {
    previous_double_bracket = 0;
    insert_char(ch, word);
  }
}

void validate_char_for_prev_bracket_end(int k, char keys_array[][k], int v, char values_array[][v], char *word, int loop_index) {
  if (ch != '}') {
    previous_char_bracket = 0;
    insert_char(ch, word);
  } else if (ch == '}') {
    if (getting_loop_tag) {
      end_interpolation_word(word);
      getting_loop_tag = 0;
      render_loop(k, keys_array, v, values_array, word, 0, atoi(word), ftell(file));
    } else {
      end_interpolation_word(word);
      interpolate_word(k, keys_array, v, values_array, word, loop_index);
    }
  }
}

void render_loop(int k, char keys_array[][k], int v, char values_array[][v], char *word, int index, int count, int start_index) {
  if (count <= 0 || index >= count) {
    fseek(file, -1, SEEK_CUR);
    return;
  }
  fseek(file, start_index, SEEK_SET);
  while ((ch = getc(file)) != EOF) {
    if (reached_loop_end) {
      reached_loop_end = 0;
      render_loop(k, keys_array, v, values_array, word, index + 1, count, start_index);
      break;
    } else {
      validate_character(k, keys_array, v, values_array, word, index);
    }
  }
}

void insert_char(char c, char *word) {
  if (c != ' ') {
    word[insert_index] = c;
    insert_index++;
  }
}

void end_interpolation_word(char *word) {
  word[insert_index] = '\0';
  insert_index = 0;
  previous_char_bracket = 0;
  searching_for_start = 1;
}

void render_partial(int k, char keys_array[][k], int v, char values_array[][v], char *word) {
  previous_double_bracket = 0;
  rendering_partial = 0;
  FILE *partial;
  partial = fopen(word, "r");
  if (partial == NULL) {
    rendering_partial = 0;
    return;
  }
  while ((ch = getc(partial)) != EOF) {
    validate_character(k, keys_array, v, values_array, word, -1);
  }
  fclose(partial);
}

void interpolate_word(int k, char keys_array[][k], int v, char values_array[][v], char *word, int loop_index) {
  if (rendering_partial) {
    render_partial(k, keys_array, v, values_array, word);
    return;
  }
  if (strcmp(word, "-") == 0) {
    reached_loop_end = 1;
    strcpy(word, "");
    insert_index = 0;
    return;
  }
  char key[strlen(word) + 20];
  strcpy(key, "");
  strcpy(key, word);
  if (loop_index >= 0 && word[0] == '_') {
    char s_nr[20];
    sprintf(s_nr, "%d", loop_index);
    strcat(key, s_nr);
  }
  char *value = find_json_value(key, k, keys_array, v, values_array);
  int i = 0;
  while ((word_ch = value[i]) != '\0') {
    printf("%c", word_ch);
    i++;
  }
  strcpy(word, "");
  insert_index = 0;
}
