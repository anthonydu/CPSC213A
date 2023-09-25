#include <stdlib.h>

int  a;
int* b;

void foo () {
  a = 0; 
  b = malloc (10*sizeof(int));
  b[a] = a;
}
