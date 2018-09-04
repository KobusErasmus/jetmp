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
  json_to_array(json);
  _Bool result = !strcmp("Tom Blue", find_json_value("name"));
  assert_true(result, "Find JSON string value");
  result = !strcmp("Male", find_json_value("Gender"));
  assert_true(result, "Find JSON string value");
}

void test_find_json_value_number() {
  _Bool result = !strcmp("32", find_json_value("age"));
  assert_true(result, "Find JSON number value");
  result = !strcmp("67.0", find_json_value("height"));
  assert_true(result, "Find JSON number value");
}

void test_no_key_returns_empty_string() {
  _Bool result = !strcmp("", find_json_value("nonexistent"));
  assert_true(result, "Find JSON nonexistent key returns empty string");
}
