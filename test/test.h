#include <string.h>

void print_fail(char description[]);
void assert_true(_Bool value, char description[]);
void assert_false(_Bool value, char description[]);
void assert_int_equal(int expected, int actual, char description[]);

void print_fail(char description[]) {
  printf("FAIL: %s\n", description);
}

void assert_true(_Bool value, char description[]) {
  if (!value) {
    print_fail(description);
    exit(0);
  }
}

void assert_false(_Bool value, char description[]) {
  if (value) {
    print_fail(description);
    exit(0);
  }
}

void assert_int_equal(int expected, int actual, char description[]) {
  char output[100];
  if (expected != actual) {
    sprintf(output, "%s - Expected: %d, actual: %d", description, expected, actual);
    print_fail(output);
    exit(0);
  }
}
