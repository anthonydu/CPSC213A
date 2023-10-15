struct S {
  int x[2];
  int *y;
  struct S *z;
};

int i;
int v0, v1, v2, v3;
struct S s;

void foo() {
  v0 = s.x[i];
  v1 = s.y[i];
  v2 = s.z->x[i];
  v3 = s.z->z->y[i];
}