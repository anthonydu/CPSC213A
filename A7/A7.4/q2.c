#include <stdio.h>

int x[8] = {1, 2, 3, -1, -2, 0, 184, 340057058};
int y[8];

int f(int a) {
  int retval = 0;
  for (int r1 = a; r1;) {
    if (0x80000000 & r1) {
      retval++;
    }
    r1 *= 2;
  }
  return retval;
}

int main() {
  int _what_is_the_purpose_of_this_local_variable_question_mark;
  for (int i = 8; i;) {
    i--;
    y[i] = f(x[i]);
  }
  for (int i = 0; i < 8; i++) {
    printf("%d\n", x[i]);
  }
  for (int i = 0; i < 8; i++) {
    printf("%d\n", y[i]);
  }
}