#include <stdlib.h>
#include <stdio.h>

/*
 * Class A
 */

void A_ping(void*);
void A_pong(void*);

struct A_class {
  void (*ping)(void*);
  void (*pong)(void*);
} A_class_table = {A_ping, A_pong};

struct A {
  struct A_class *class;
  int i;
};

void A_ctor(void *this_v, int i) {
  struct A *this = this_v;
  this->i = i;
}

struct A *new_A(int i) {
  struct A *this = malloc(sizeof(*this));
  this->class = &A_class_table;
  A_ctor(this, i);
  return this;
}

void A_ping(void *this_v) {
  struct A *this = this_v;
  printf("A_ping %d\n", this->i);
}

void A_pong(void *this_v) {
  struct A *this = this_v;
  printf("A_pong %d\n", this->i);
}


/*
 * Class B extends A
 */

void B_ping(void*);
void B_wiff(void*);

struct B_class {
  void (*ping)(void*);
  void (*pong)(void*);
  void (*wiff)(void*);
} B_class_table = {B_ping, A_pong, B_wiff};

struct B {
  struct B_class *class;
  int    i;
};

void B_ctor(void *this, int i) {
  A_ctor(this, i);
}

struct B *new_B(int i) {
  struct B *this = malloc(sizeof((*this)));
  this->class = &B_class_table;
  A_ctor(this, i);
  return this;
}

void B_ping(void *this_v) {
  struct B *this = this_v;
  printf ("B_ping %d\n", this->i);
}

void B_wiff(void *this_v) {
  struct B *this = this_v;
  printf ("B_wiff %d\n", this->i);
}



/*
 * Class C extends B
 */

void C_ping(void*);

struct C_class {
  void (*ping)(void*);
  void (*pong)(void*);
  void (*wiff)(void*);
} C_class_table = {C_ping, A_pong, B_wiff};

struct C {
  struct C_class *class;
  int    i;
  int    id;
};

void C_ctor(void *this_v, int i, int id) {
  struct C *this = this_v;
  B_ctor(this, i);
  this->id = id;
}

struct C *new_C(int i, int id) {
  struct C *this = malloc(sizeof(*this));
  this->class = &C_class_table;
  C_ctor(this, i, id);
  return this;
}

void C_ping(void *this_v) {
  struct C *this = this_v;
  printf("C_ping %d, %d\n", this->i, this->id);
}


/*
 * Main
 */

void foo(void *a_v) {
  struct A *a = a_v;
  a->class->ping(a);
  a->class->pong(a);
}

void bar() {
  foo(new_A(10));
  foo(new_B(20));
  foo(new_C(30, 100));
}

int main (int argc, char** argv) {
  bar();
}
