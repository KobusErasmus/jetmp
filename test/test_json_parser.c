#include "../src/json_parser.h"
#include "test.h"

char json[] = "{ \"name\":\"Tom Blue\","
              "\"age\": 32, \"Gender\" : Male,"
              " \"height\"   :67.0}";
void test_find_json_value_string();
void test_find_json_value_number();
void test_no_key_returns_empty_string();

int main() {
  test_find_json_value_string();
  test_find_json_value_number();
  printf("All tests pass for: json_parser.h\n");
}

void test_find_json_value_string() {
  int key_length = 51;
  int value_length = 1001;
  char keys_array[100][key_length];
  char values_array[100][value_length];
  char word[value_length];
  json_to_array(json, 51, keys_array, value_length, values_array, word);
  _Bool result = !strcmp("Tom Blue", find_json_value("name", key_length, keys_array, value_length, values_array));
  assert_true(result, "Find JSON string value");
  result = !strcmp("Male", find_json_value("Gender", key_length, keys_array, value_length, values_array));
  assert_true(result, "Find JSON string value");
}

void test_find_json_value_number() {
  int key_length = 51;
  char keys_array[100][key_length];
  int value_length = 1001;
  char values_array[100][value_length];
  char word[value_length];
  json_to_array(json, 51, keys_array, value_length, values_array, word);
  _Bool result = !strcmp("32", find_json_value("age", key_length, keys_array, value_length, values_array));
  assert_true(result, "Find JSON number value 32");
  result = !strcmp("67.0", find_json_value("height", key_length, keys_array, value_length, values_array));
  assert_true(result, "Find JSON number value 67");
}

void test_no_key_returns_empty_string() {
  int key_length = 51;
  char keys_array[100][key_length];
  int value_length = 1001;
  char values_array[100][value_length];
  _Bool result = !strcmp("", find_json_value("nonexistent", key_length, keys_array, value_length, values_array));
  assert_true(result, "Find JSON nonexistent key returns empty string");
}
