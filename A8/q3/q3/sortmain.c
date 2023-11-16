#include <stdio.h>
#include <stdlib.h>

#include "int_element.h"
#include "str_element.h"
#include "element.h"

/* If the string is numeric, return an int_element. Otherwise return a str_element. */
struct element *parse_string(char *str) {
  char *endp;
  /* strtol returns a pointer to the first non-numeric character in endp.
     If it gets to the end of the string, that character will be the null terminator. */
  int value = strtol(str, &endp, 10);
  if(str[0] != '\0' && endp[0] == '\0') {
    /* String was non-empty and strtol conversion succeeded - integer */
    return (struct element *)int_element_new(value);
  } else {
    return (struct element *)str_element_new(str);
  }
}

int main(int argc, char **argv) {
  /* TODO: Read elements into a new array using parse_string */
  /* TODO: Sort elements with qsort */
  printf("Sorted: ");
  /* TODO: Print elements, separated by a space */
  printf("\n");
}
