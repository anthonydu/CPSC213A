// exploit by running:
// gcc vuln.c -o vuln && xxd -r -p proof.hex | ./vuln

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct str {
  int length;
  char* str;
};

void halt();
void print();
void proof();
int addr(void* p);
void* get(int addr);

int stack[130];
int* stacktop = stack + 129;
struct str str1 = {30, "Welcome! Please enter a name:\n"};
struct str str2 = {11, "Good luck, "};
struct str str3 = {43, "The secret phrase is \"squeamish ossifrage\"\n"};

int main() {
  stacktop -= 1;
  *stacktop = addr(&halt);
  stacktop -= 32;
  stacktop -= 1;
  *stacktop = addr(&str1);
  print();
  stacktop += 1;
  int size = read(0, stacktop, 256);
  // for (int i = 0; i < 130; i++) {
  //   printf("%p\n", stack[i]);
  // }
  stacktop -= 1;
  *stacktop = addr(&str2);
  print();
  stacktop += 1;
  write(1, stacktop, size);
  stacktop += 32;
  void (*return_address)() = get(*stacktop);
  stacktop += 1;
  (*return_address)();
}

void halt() {
  printf("HALT instruction executed.\n");
  exit(0);
}

void print() {
  struct str* str = get(*stacktop);
  write(1, str->str, str->length);
}

void proof() {
  stacktop -= 1;
  *stacktop = addr(&str3);
  print();
  halt();
}

// =============helpers=============

int addr(void* p) {
  if (p == &halt) return 0x0e100000;
  if (p == &print) return 0x70100000;
  if (p == &proof) return 0x00180000;
  if (p == &str1) return 0x00200000;
  if (p == &str2) return 0x28200000;
  if (p == &str3) return 0x3c200000;
}

void* get(int addr) {
  if (addr == 0x0e100000) return &halt;
  if (addr == 0x70100000) return &print;
  if (addr == 0x00180000) return &proof;
  if (addr == 0x00200000) return &str1;
  if (addr == 0x28200000) return &str2;
  if (addr == 0x3c200000) return &str3;
}