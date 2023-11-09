#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* simulate exec using the C function system */
/* this function is not in the assembly */
int exec(char *buffer, int size) {
  /* buffer might not be null-terminated, but system() wants a null-terminated string,
     so our emulated exec adds null-termination here using a temporary variable */
  char *str = malloc(size + 1);
  memcpy(str, buffer, size);
  str[size] = 0;
  int res = system(str);
  free(str);
  return res;
}

char buf[64];
int main() {
  int size = read(0, buf, sizeof(buf));
  exec(buf, size);
}
