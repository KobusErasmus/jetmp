#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_KEYS 100
#define MAX_KEY_LENGTH 51
#define MAX_VALUE_LENGTH 501

char keys_array[MAX_KEYS][MAX_KEY_LENGTH];
char values_array[MAX_KEYS][MAX_VALUE_LENGTH];
_Bool searching_for_key = 1;
_Bool searching_for_value = 0;
_Bool searching_for_colon = 0;
_Bool adding_key = 0;
_Bool adding_value = 0;
_Bool is_value_a_string = 0;
char word[MAX_VALUE_LENGTH];
char character;
int word_index = 0;
int value_index = 0;

void json_to_array(char json[]);
char* find_json_value(char key[]);
void add_char_to_word(char c);
void end_adding_key();
void end_adding_value();
void search_for_key();
void add_key_char();
void search_for_colon();
void search_for_value();
void add_value_char();
void analyse_character();
_Bool reached_end_of_word();

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
  if (character != '\"') {
    add_char_to_word(character);
  } else {
    end_adding_key();
    adding_key = 0;
    searching_for_colon = 1;
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
  if (!reached_end_of_word()) {
    add_char_to_word(character);
    return;
  }
  end_adding_value();
  searching_for_key = 1;
  adding_value = 0;
  is_value_a_string = 0;
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
