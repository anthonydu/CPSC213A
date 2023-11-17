#include <stdio.h>
#include <stdlib.h>

#include "element.h"
#include "int_element.h"
#include "refcount.h"
#include "str_element.h"

/* If the string is numeric, return an int_element. Otherwise return a
 * str_element. */
struct element *parse_string(char *str) {
  char *endp;
  /* strtol returns a pointer to the first non-numeric character in endp.
     If it gets to the end of the string, that character will be the null
     terminator. */
  int value = strtol(str, &endp, 10);
  if (str[0] != '\0' && endp[0] == '\0') {
    /* String was non-empty and strtol conversion succeeded - integer */
    return (struct element *)int_element_new(value);
  } else {
    return (struct element *)str_element_new(str);
  }
}

int compare(struct element **obj1, struct element **obj2) {
  return (*obj1)->class->compare(*obj1, *obj2);
}

int main(int argc, char **argv) {
  /* TODO: Read elements into a new array using parse_string */
  struct element *arr[argc - 1];
  for (int i = 0; i < argc - 1; i++) {
    arr[i] = parse_string(argv[i + 1]);
  }
  /* TODO: Sort elements with qsort */
  printf("Sorted: ");
  qsort(arr, argc - 1, sizeof(struct element *), compare);
  /* TODO: Print elements, separated by a space */
  for (int i = 0; i < argc - 1; i++) {
    arr[i]->class->print(arr[i]);
    printf(" ");
  }
  printf("\n");
  for (int i = 0; i < argc - 1; i++) {
    rc_free_ref(arr[i]);
  }
}
