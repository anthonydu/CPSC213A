#include <stdlib.h>
#include <stdio.h>
#include "list.h"

int isEven (element_t av) {
  int *a = av;
  return ! (*a & 1);
}

void add (element_t* rv, element_t av, element_t bv) {
  int *a = av, *b = bv, **r = (int**) rv;
  if (*r == NULL)
    *r = malloc(sizeof(int));
  **r = *a + *b;
}

void inc (element_t* rv, element_t av) {
  int* a = av, **r = (int**) rv;
  if (*r == NULL)
    *r = malloc(sizeof(int));
  **r = *a + 1;
}

void print (element_t ev) {
  int* e = ev;
  printf ("%d\n", *e);
}

int** starify(int* inArray, int n) {
  int** outArray = malloc (n * sizeof(int*));
  for (int i=0; i<n; i++)
    outArray[i] = &inArray[i];
  return outArray;
}

int main() {
  int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
  int b[] = {11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
  int n   = sizeof(a) / sizeof(a[0]);
  int** ap = starify(a, n);
  int** bp = starify(b, n);

  struct list* l0 = list_create();
  list_append_array (l0, (element_t*) ap, n);

  struct list* l1 = list_create();
  list_append_array (l1, (element_t*) bp, n);

  struct list* l2 = list_create();
  list_filter (isEven, l2, l0);
  printf ("filter:\n");
  list_foreach (print, l2);

  struct list* l3 = list_create();
  list_map1 (inc, l3, l0);
  printf ("map1:\n");
  list_foreach (print, l3);

  struct list* l4 = list_create();
  list_map2 (add, l4, l3, l2);
  printf ("map2:\n");
  list_foreach (print, l4);

  int s = 0, *sp = &s;
  list_foldl (add, (element_t*) &sp, l4);
  printf ("fold: %d\n", s);

  list_foreach (free, l3);  // free elements allocated by inc in map1
  list_foreach (free, l4);  // free elements allocated by add in map2

  list_destroy (l0);
  list_destroy (l1);
  list_destroy (l2);
  list_destroy (l3);
  list_destroy (l4);

  free(ap);
  free(bp);
}
