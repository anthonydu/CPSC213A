#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct str {
  int length;
  char* str;
};

void* stack[130];
void** stacktop = stack + 129;
struct str str1 = {30, "Welcome! Please enter a name:\n"};
struct str str2 = {11, "Good luck, "};
struct str str3 = {43, "The secret phrase is \"squeamish ossifrage\""};

void halt();
void print();
void proof();

int main() {
  stacktop -= 1;
  *stacktop = &halt;
  stacktop -= 32;
  stacktop -= 1;
  *stacktop = &str1;
  print();
  stacktop += 1;
  int size = read(0, stacktop, 256);
  stacktop -= 1;
  *stacktop = &str2;
  print();
  stacktop += 1;
  write(1, stacktop, size);
  stacktop += 32;
  void (*return_address)() = *stacktop;
  stacktop += 1;
  (*return_address)();
}

void halt() {
  printf("HALT instruction executed.\n");
  exit(0);
}

void print() {
  struct str* str = *stacktop;
  write(1, str->str, str->length);
}

void proof() {
  stacktop -= 1;
  *stacktop = &str3;
  print();
  exit(0);
}