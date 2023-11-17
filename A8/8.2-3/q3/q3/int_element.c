#include "int_element.h"

#include <stdio.h>
#include <stdlib.h>

#include "refcount.h"

/* TODO: Implement all public int_element functions, including element
interface functions. You may add your own private functions here too. */

struct int_element_class {
  void (*print)(struct element *);
  int (*compare)(struct element *, struct element *);
  struct int_element *(*int_element_new)(int);
  int (*int_element_get_value)(struct int_element *);
  int (*is_int_element)(struct element *);
};

void int_element_print(struct element *);

int int_element_compare(struct element *, struct element *);

void int_element_free(struct int_element *);

struct int_element_class int_element_obj = {int_element_print,
                                            int_element_compare,
                                            int_element_new,
                                            int_element_get_value,
                                            is_int_element};

struct int_element {
  struct int_element_class *class;
  int *value;
};

/* Static constructor that creates new integer elements. */
struct int_element *int_element_new(int value) {
  struct int_element *obj
      = rc_malloc(sizeof(struct int_element), int_element_free);
  obj->class = &int_element_obj;
  obj->value = malloc(sizeof(int));
  *(obj->value) = value;
  return obj;
}

void int_element_print(struct element *obj) {
  printf("%d", int_element_get_value(obj));
}

int int_element_compare(struct element *obj1, struct element *obj2) {
  if (!is_int_element(obj2)) return -1;
  if (int_element_get_value(obj1) == int_element_get_value(obj2)) return 0;
  if (int_element_get_value(obj1) > int_element_get_value(obj2)) return 1;
  if (int_element_get_value(obj1) < int_element_get_value(obj2)) return -1;
}

/* Static function that obtains the value held in an int_element. */
int int_element_get_value(struct int_element *obj) {
  return *(obj->value);
}

/* Static function that determines whether this is an int_element. */
int is_int_element(struct element *obj) {
  return obj->class == &int_element_obj;
}

void int_element_free(struct int_element *obj) {
  free(obj->value);
}