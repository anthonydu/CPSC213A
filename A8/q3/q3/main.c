#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "element.h"
#include "int_element.h"
#include "str_element.h"
#include "list.h"
#include "tree.h"
#include "refcount.h"

/**
 * Your solution cannot depend in any way on the implementation of this function.
 * That is, your program must work with any arbitrary selection function.
 */
int includeElement() {
  static int count=0;
  return (++count) % 2;
}

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

void test_int() {
  /* Basic test of int_element */
  struct element *e = (struct element *)int_element_new(42);
  e->class->print(e);
  printf("\n");
  rc_free_ref(e);
}

void test_str() {
  /* Basic test of str_element */
  char *s = strdup("Hello, World!");
  struct element *e = (struct element *)str_element_new(s);
  free(s);
  e->class->print(e);
  printf("\n");
  rc_free_ref(e);
}

int main(int argc, char **argv) {
  test_int();
  test_str();

  struct tree *t = tree_new();
  struct list *l = list_new();

  /* Parse input arguments */
  for (int i=0; i<argc-1; i++) {
    struct element *e = parse_string(argv[i+1]);
    list_add_element(l, e);
    rc_free_ref(e);
  }

  /* Put some of the elements on the tree */
  for(struct list_node *n = list_get_head(l); n != NULL; n = list_node_get_next(n)) {
    if(includeElement())
      tree_insert_node(t, list_node_get_element(n));
  }
  printf("Tree:\n");
  tree_ordered_print(t);

  /* Print out only the integers */
  printf("List ints:");
  for(struct list_node *n = list_get_head(l); n != NULL; n = list_node_get_next(n)) {
    struct element *e = list_node_get_element(n);
    if(is_int_element(e)) {
      printf(" %d", int_element_get_value((struct int_element *)e));
    }
  }
  printf("\n");

  /* Print out only the strings */
  printf("List strings:");
  for(struct list_node *n = list_get_head(l); n != NULL; n = list_node_get_next(n)) {
    struct element *e = list_node_get_element(n);
    if(is_str_element(e)) {
      printf(" %s", str_element_get_value((struct str_element *)e));
    }
  }
  printf("\n");

  list_delete(l);

  printf("Tree after deleting list:\n");
  tree_ordered_print(t);
  tree_delete(t);
}
