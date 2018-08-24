#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Define variables
char keys_values[50][101];
_Bool searching_for_key = 1;
_Bool searching_for_value = 0;
_Bool searching_for_colon = 0;
_Bool adding_key = 0;
_Bool adding_value = 0;
_Bool is_value_a_string = 0;
char word[101];
char character;
int word_index = 0;
int value_index = 0;

// Define functions
void json_to_array(char json[]);
char* find_json_value(char key[], char json[]);
void add_char_to_word(char c);
void end_found_key();
void search_for_key();
void add_key_char();
void search_for_colon();
void search_for_value();
void add_value_char();

void json_to_array(char json[]) {
  int i;
  for (i = 0; i < strlen(json); i++) {
    character = json[i];
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
    end_found_key();
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
  if (is_value_a_string && character == '\"') {
    end_found_key();
    searching_for_key = 1;
    adding_value = 0;
    is_value_a_string = 0;
  } else if (!is_value_a_string && (character == ' ' || character == ',' || character == '}')) {
    end_found_key();
    searching_for_key = 1;
    adding_value = 0;
    is_value_a_string = 0;
  } else {
    add_char_to_word(character);
  }
}

void add_char_to_word(char c) {
  word[word_index] = c;
  word_index++;
}

void end_found_key() {
  word[word_index] = '\0';
  strcpy(keys_values[value_index], word);
  value_index++;
  strcpy(word, "");
  word_index = 0;
}

char* find_json_value(char key[], char json[]) {
  int i;
  for (i = 0; i < 50; i++) {
    if (strlen(keys_values[i]) == 0) return "";
    if (strcmp(key, keys_values[i]) == 0) {
      return keys_values[i + 1];
    }
  }
  return "";
}