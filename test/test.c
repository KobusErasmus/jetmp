#include "../lib/jetmp.h"
#include "test.h"

char json[] = "{ \"name\":\"Tom Blue\", \"age\": 32 }";
void test_find_json_value_string();
void test_find_json_value_integer();

int main() {
  test_find_json_value_string();
  test_find_json_value_integer();
  printf("All tests pass\n");
}

void test_find_json_value_string() {
  json_to_array(json);
  _Bool result = !strcmp("Tom Blue", find_json_value("name", json));
  assert_true(result, "Find JSON string value");
}

void test_find_json_value_integer() {
  _Bool result = !strcmp("32", find_json_value("age", json));
  assert_true(result, "Find JSON integer value");
}
