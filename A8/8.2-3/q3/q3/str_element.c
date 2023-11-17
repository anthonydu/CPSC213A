#include "str_element.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "refcount.h"

/* TODO: Implement all public str_element functions, including element
interface functions. You may add your own private functions here too. */

struct str_element_class {
  void (*print)(struct element *);
  int (*compare)(struct element *, struct element *);
  struct str_element *(*str_element_new)(char *);
  int (*str_element_get_value)(struct str_element *);
  int (*is_str_element)(struct element *);
};

void str_element_print(struct element *);

int str_element_compare(struct element *, struct element *);

void str_element_free(struct str_element *);

struct str_element_class str_element_obj = {str_element_print,
                                            str_element_compare,
                                            str_element_new,
                                            str_element_get_value,
                                            is_str_element};

struct str_element {
  struct str_element_class *class;
  char *value;
};

/* Static constructor that creates new integer elements. */
struct str_element *str_element_new(char *value) {
  struct str_element *obj
      = rc_malloc(sizeof(struct str_element), str_element_free);
  obj->class = &str_element_obj;
  obj->value = strdup(value);
  return obj;
}

void str_element_print(struct element *obj) {
  printf(str_element_get_value(obj));
}

int str_element_compare(struct element *obj1, struct element *obj2) {
  if (!is_str_element(obj2)) return 1;
  return strcmp(str_element_get_value(obj1), str_element_get_value(obj2));
}

/* Static function that obtains the value held in an str_element. */
char *str_element_get_value(struct str_element *obj) {
  return obj->value;
}

/* Static function that determines whether this is an str_element. */
int is_str_element(struct element *obj) {
  return obj->class == &str_element_obj;
}

void str_element_free(struct str_element *obj) {
  free(obj->value);
}