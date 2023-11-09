#include <unistd.h>

char buf[16];
int main() {
  read(0, buf, 16);
  write(1, buf, 16);
}
