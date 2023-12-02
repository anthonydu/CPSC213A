#include <stdio.h>
#include <stdlib.h>

#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_THREADS 3
uthread_t threads[NUM_THREADS];
int num_as_printed = 0;
uthread_mutex_t mutex;
uthread_cond_t cond;

void randomStall() {
  int i, r = random() >> 16;
  while (i++ < r)
    ;
}

void waitForAllOtherThreads() {
  uthread_mutex_lock(mutex);
  num_as_printed++;
  if (num_as_printed == NUM_THREADS) {
    uthread_cond_broadcast(cond);
  } else {
    uthread_cond_wait(cond);
  }
  uthread_mutex_unlock(mutex);
}

void* p(void* v) {
  randomStall();
  printf("a\n");
  waitForAllOtherThreads();
  printf("b\n");
  return NULL;
}

int main(int arg, char** arv) {
  uthread_init(4);
  mutex = uthread_mutex_create();
  cond = uthread_cond_create(mutex);
  for (int i = 0; i < NUM_THREADS; i++) threads[i] = uthread_create(p, NULL);
  for (int i = 0; i < NUM_THREADS; i++) uthread_join(threads[i], NULL);
  printf("------\n");
}