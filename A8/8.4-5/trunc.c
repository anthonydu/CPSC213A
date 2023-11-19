// gcc trunc.c -o trunc

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

int is_int(char* str) {
  char* endptr;
  strtol(str, &endptr, 10);
  int result = str[0] != 0 && endptr[0] == 0;
  endptr = NULL;
  return result;
}

void to_int(int* out, char* str) {
  *out = strtol(str, NULL, 10);
}

int is_str(char* str) {
  return !is_int(str);
}

void trunc(char** out, int len, char* str) {
  char* result = malloc(len);
  *out = strncpy(result, str, len);
  result = NULL;
}

void print_vertical(char* str) {
  printf("%s\n", str);
}

void print_horizontal(char* str) {
  printf("%s ", str);
}

void max(int* max_p, int max, int num2) {
  *max_p = max > num2 ? max : num2;
}

int main(int argc, char** argv) {
  struct list* args = list_create();
  list_append_array(args, argv + 1, argc - 1);

  struct list* int_strs = list_create();
  list_filter(is_int, int_strs, args);
  struct list* strs = list_create();
  list_filter(is_str, strs, args);
  list_destroy(args);
  args = NULL;

  struct list* ints = list_create();
  list_map1(to_int, ints, int_strs);
  list_destroy(int_strs);
  int_strs = NULL;

  struct list* results = list_create();
  list_map2(trunc, results, ints, strs);
  list_destroy(strs);
  strs = NULL;

  list_foreach(print_vertical, results);
  list_foreach(print_horizontal, results);
  list_foreach(free, results);
  list_destroy(results);
  results = NULL;

  int max_num = NULL;
  list_foldl(max, &max_num, ints);
  printf("\n%d\n", max_num);
  list_destroy(ints);
  ints = NULL;
}