#include "./json_parser.h"
#include "./jetmp_definitions.h"

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("Usage: %s filename json\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  file = fopen(argv[1], "r");
  if (file == NULL) {
    printf("Cannot open %s\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  json_to_array(argv[2]);
  interpolate_string();
}

void interpolate_string() {
  while ((ch = getc(file)) != EOF) {
    validate_character();
  }
  fclose(file);
}

void validate_character() {
  if (searching_for_start) {
    validate_character_for_start();
  } else {
    validate_character_for_end();
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

void validate_character_for_end() {
  if (previous_double_bracket && ch == '>') {
    rendering_partial = 1;
  } else if (!previous_char_bracket && ch == '}') {
    previous_char_bracket = 1;
    previous_double_bracket = 0;
  } else if (previous_char_bracket) {
    previous_double_bracket = 0;
    validate_char_for_prev_bracket_end();
  } else {
    previous_double_bracket = 0;
    insert_char(ch);
  }
}

void validate_char_for_prev_bracket_end() {
  if (ch != '}') {
    previous_char_bracket = 0;
    insert_char(ch);
  } else if (ch == '}') {
    end_interpolation_word();
    interpolate_word();
  }
}

void insert_char(char c) {
  if (c != ' ') {
    word[insert_index] = c;
    insert_index++;
  }
}

void end_interpolation_word() {
  word[insert_index] = '\0';
  insert_index = 0;
  previous_char_bracket = 0;
  searching_for_start = 1;
}

void render_partial() {
  previous_double_bracket = 0;
  rendering_partial = 0;
  FILE *partial;
  partial = fopen(word, "r");
  if (partial == NULL) {
    rendering_partial = 0;
    return;
  }
  while ((ch = getc(partial)) != EOF) {
    validate_character();
  }
  fclose(partial);
}

void interpolate_word() {
  if (rendering_partial) {
    render_partial();
    return;
  }
  char *value = find_json_value(word);
  int i = 0;
  while ((word_ch = value[i]) != '\0') {
    printf("%c", word_ch);
    i++;
  }
  strcpy(word, "");
  insert_index = 0;
}
