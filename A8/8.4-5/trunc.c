// gcc trunc.c -o trunc

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

int is_int(char* str) {
  char* endptr;
  strtol(str, &endptr, 10);
  return str[0] != 0 && endptr[0] == 0;
}

void to_int(int* out, char* str) {
  char* endptr;
  *out = strtol(str, &endptr, 10);
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

  struct list* ints = list_create();
  list_map1(to_int, ints, int_strs);

  struct list* strs = list_create();
  list_filter(is_str, strs, args);

  struct list* results = list_create();
  list_map2(trunc, results, ints, strs);
  list_foreach(print_vertical, results);
  list_foreach(print_horizontal, results);

  int* max_num = malloc(sizeof(int*));
  list_foldl(max, max_num, ints);
  printf("\n%d\n", *max_num);

  list_foreach(free, results);
  list_destroy(args);
  list_destroy(int_strs);
  list_destroy(ints);
  list_destroy(strs);
  list_destroy(results);
  free(max_num);

  args = NULL;
  int_strs = NULL;
  ints = NULL;
  strs = NULL;
  results = NULL;
  max_num = NULL;
}