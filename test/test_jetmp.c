#include "../lib/jetmp.h"
#include "test.h"

char json[] = "{ \"name\":\"Tom Blue\","
              "\"age\": 32, \"Gender\" : Male,"
              " \"height\"   :67.0}";
void test_interpolation();

int main() {
  test_interpolation();
  printf("All tests pass for: jetmp.h\n");
}

void test_interpolation() {
  _Bool result = !strcmp("test", interpolated_string());
  assert_true(result, "Test interpolated string");
}
