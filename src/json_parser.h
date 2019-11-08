#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./json_parser_definitions.h"

void json_to_array(char json[], int k, char keys_array[][k], int v, char values_array[][v], char *word) {
  int i;
  for (i = 0; i < strlen(json); i++) {
    character = json[i];
    analyse_character(k, keys_array, v, values_array, word);
  }
}

void analyse_character(int k, char keys_array[][k], int v, char values_array[][v], char *word) {
  if (searching_for_key) {
    search_for_key();
  } else if (adding_key) {
    add_key_char(k, keys_array, word);
  } else if (searching_for_colon) {
    search_for_colon();
  } else if (searching_for_value) {
    search_for_value(word);
  } else if (adding_value) {
    add_value_char(v, values_array, word);
  }
}

void search_for_key() {
  if (character != '\"') return;
  searching_for_key = 0;
  adding_key = 1;
}

void add_key_char(int k, char keys_array[][k], char *word) {
  if (!previous_ch_escape && character == '\\') {
    previous_ch_escape = 1;
  } else if (previous_ch_escape && character == '\"') {
    add_char_to_word(character, word);
    previous_ch_escape = 0;
  } else if (character != '\"') {
    if (previous_ch_escape) add_char_to_word('\\', word);
    add_char_to_word(character, word);
    previous_ch_escape = 0;
  } else {
    end_adding_key(k, keys_array, word);
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

void search_for_value(char *word) {
  if (character == '\"') {
    is_value_a_string = 1;
    adding_value = 1;
    searching_for_value = 0;
  } else if (character != ' ') {
    is_value_a_string = 0;
    adding_value = 1;
    searching_for_value = 0;
    add_char_to_word(character, word);
  }
}

void add_value_char(int v, char values_array[][v], char *word) {
  if (!previous_ch_escape && character == '\\') {
    previous_ch_escape = 1;
  } else if (previous_ch_escape && character == '\"') {
    add_char_to_value(character, word);
    previous_ch_escape = 0;
  } else if (!reached_end_of_word()) {
    if (previous_ch_escape) add_char_to_value('\\', word);
    add_char_to_value(character, word);
    previous_ch_escape = 0;
  } else {
    end_adding_value(v, values_array, word);
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

void add_char_to_word(char c, char *word) {
  word[word_index] = c;
  word_index++;
}

void add_char_to_value(char c, char *word) {
  if (!escape_html) return add_char_to_word(c, word);
  switch(c) {
    case '&': add_string_to_word("&amp;", word); break;
    case '<': add_string_to_word("&lt;", word); break;
    case '>': add_string_to_word("&gt;", word); break;
    case '\"': add_string_to_word("&quot;", word); break;
    case '\'': add_string_to_word("&#39;", word); break;
    default: add_char_to_word(c, word);
  }
}

void add_string_to_word(char *string, char *word) {
  int i;
  int l = strlen(string);
  for (i = 0; i < l; i++) {
    add_char_to_word(string[i], word);
  }
}

void end_adding_key(int k, char keys_array[][k], char *word) {
  word[word_index] = '\0';
  strcpy(keys_array[value_index], word);
  strcpy(word, "");
  word_index = 0;
}

void end_adding_value(int v, char values_array[][v], char *word) {
  word[word_index] = '\0';
  strcpy(values_array[value_index], word);
  value_index++;
  strcpy(word, "");
  word_index = 0;
}

char* find_json_value(char key[], int k, char keys_array[][k], int v, char values_array[][v]) {
  int i;
  for (i = 0; i < max_keys; i++) {
    if (strcmp(key, keys_array[i]) == 0) {
      return values_array[i];
    }
  }
  return "";
}
