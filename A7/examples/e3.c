#include <unistd.h>

int main() {
  char buf[16];
  read(0, buf, 16);
  write(1, buf, 16);
}
