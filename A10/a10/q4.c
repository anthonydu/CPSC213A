#include <stdio.h>
#include <stdlib.h>

#include "uthread.h"
#include "uthread_mutex_cond.h"

uthread_t t0, t1, t2;
int print_state = 0;
uthread_mutex_t mutex;
uthread_cond_t cond;

void randomStall() {
  int i, r = random() >> 16;
  while (i++ < r)
    ;
}

void* p0(void* v) {
  randomStall();
  uthread_mutex_lock(mutex);
  printf("zero\n");
  print_state++;
  uthread_cond_broadcast(cond);
  uthread_mutex_unlock(mutex);
  return NULL;
}

void* p1(void* v) {
  randomStall();
  uthread_mutex_lock(mutex);
  while (print_state != 1) uthread_cond_wait(cond);
  printf("one\n");
  print_state++;
  uthread_cond_broadcast(cond);
  uthread_mutex_unlock(mutex);
  return NULL;
}

void* p2(void* v) {
  randomStall();
  uthread_mutex_lock(mutex);
  while (print_state != 2) uthread_cond_wait(cond);
  printf("two\n");
  print_state++;
  uthread_cond_broadcast(cond);
  uthread_mutex_unlock(mutex);
  return NULL;
}

int main(int arg, char** arv) {
  uthread_init(4);
  mutex = uthread_mutex_create();
  cond = uthread_cond_create(mutex);
  t2 = uthread_create(p2, NULL);
  t0 = uthread_create(p0, NULL);
  t1 = uthread_create(p1, NULL);
  uthread_join(t0, NULL);
  uthread_join(t1, NULL);
  uthread_join(t2, NULL);
  randomStall();
  uthread_mutex_lock(mutex);
  while (print_state != 3) uthread_cond_wait(cond);
  printf("three\n");
  printf("------\n");
  uthread_mutex_unlock(mutex);
}
