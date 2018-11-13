#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./json_parser_definitions.h"

void json_to_array(char json[]) {
  int i;
  for (i = 0; i < strlen(json); i++) {
    character = json[i];
    analyse_character();
  }
}

void analyse_character() {
  if (searching_for_key) {
    search_for_key();
  } else if (adding_key) {
    add_key_char();
  } else if (searching_for_colon) {
    search_for_colon();
  } else if (searching_for_value) {
    search_for_value();
  } else if (adding_value) {
    add_value_char();
  }
}

void search_for_key() {
  if (character != '\"') return;
  searching_for_key = 0;
  adding_key = 1;
}

void add_key_char() {
  if (!previous_ch_escape && character == '\\') {
    previous_ch_escape = 1;
  } else if (previous_ch_escape && character == '\"') {
    add_char_to_word(character);
    previous_ch_escape = 0;
  } else if (character != '\"') {
    if (previous_ch_escape) add_char_to_word('\\');
    add_char_to_word(character);
    previous_ch_escape = 0;
  } else {
    end_adding_key();
    adding_key = 0;
    searching_for_colon = 1;
    previous_ch_escape = 0;
  }
}

void search_for_colon() {
  if (character == ':') {
    searching_for_colon = 0;
    searching_for_value = 1;
  }
}

void search_for_value() {
  if (character == '\"') {
    is_value_a_string = 1;
    adding_value = 1;
    searching_for_value = 0;
  } else if (character != ' ') {
    is_value_a_string = 0;
    adding_value = 1;
    searching_for_value = 0;
    add_char_to_word(character);
  }
}

void add_value_char() {
  if (!previous_ch_escape && character == '\\') {
    previous_ch_escape = 1;
  } else if (previous_ch_escape && character == '\"') {
    add_char_to_value(character);
    previous_ch_escape = 0;
  } else if (!reached_end_of_word()) {
    if (previous_ch_escape) add_char_to_value('\\');
    add_char_to_value(character);
    previous_ch_escape = 0;
  } else {
    end_adding_value();
    searching_for_key = 1;
    adding_value = 0;
    is_value_a_string = 0;
    previous_ch_escape = 0;
  }
}

_Bool reached_end_of_word() {
  return (is_value_a_string && character == '\"') ||
      (!is_value_a_string && (character == ' ' ||
        character == ',' ||
        character == '}' ||
        character == '\n'));
}

void add_char_to_word(char c) {
  word[word_index] = c;
  word_index++;
}

void add_char_to_value(char c) {
  if (!escape_html) return add_char_to_word(c);
  switch(c) {
    case '&': add_string_to_word("&amp;"); break;
    case '<': add_string_to_word("&lt;"); break;
    case '>': add_string_to_word("&gt;"); break;
    case '\"': add_string_to_word("&quot;"); break;
    case '\'': add_string_to_word("&#39;"); break;
    default: add_char_to_word(c);
  }
}

void add_string_to_word(char *string) {
  int i;
  int l = strlen(string);
  for (i = 0; i < l; i++) {
    add_char_to_word(string[i]);
  }
}

void end_adding_key() {
  word[word_index] = '\0';
  strcpy(keys_array[value_index], word);
  strcpy(word, "");
  word_index = 0;
}

void end_adding_value() {
  word[word_index] = '\0';
  strcpy(values_array[value_index], word);
  value_index++;
  strcpy(word, "");
  word_index = 0;
}

char* find_json_value(char key[]) {
  int i;
  for (i = 0; i < MAX_KEYS; i++) {
    if (strcmp(key, keys_array[i]) == 0) {
      return values_array[i];
    }
  }
  return "";
}
