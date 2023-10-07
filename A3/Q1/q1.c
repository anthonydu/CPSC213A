#include <stdlib.h>
#include <stdio.h>

// YOU: Allocate these global variables, using these names
int  i, a;
int* f;
int  u[10];

int main (int argv, char** argc) {
  // Ignore this block of code
  if (argv != 11) {
    fprintf (stderr, "usage: u[0] ... u[9]\n");
    exit (EXIT_FAILURE);
  }
  for (int x=0; x<10; x++)
    u[x] = atol (argc[1 + x]);

  // YOU: Implement this code
  i  = u[7];
  i  = u[i];
  f  = &a;
  *f = 4;
  f  = &u[u[1]];
  *f = *f + u[9];

  // Ignore this block of code
  printf ("i=%d a=%d f=&u[%d] u={", i, a, f - u);
  for (int x=0; x<10; x++)
    printf("%d%s", u[x], x<9? ", ": "}\n");
  }
