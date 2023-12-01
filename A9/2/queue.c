// (C) Mike Feeley, University of BC 2018

#include <stdlib.h>
#include <signal.h>
#include "uthread.h"
#include "spinlock.h"
#include "queue.h"

struct queue_e {
  void *val, *arg;
  void (*callback) (void*, void*);
  struct queue_e *prev, *next;
};

struct queue {
  struct queue_e *front, *back, *free;
  spinlock_t      mutex;
};

queue_t queue_create() {
  queue_t q = malloc (sizeof (struct queue));
  q->front = NULL;
  q->back  = NULL;
  q->free  = NULL;
  spinlock_create (&q->mutex);
  return q;
}

void queue_destroy (queue_t q) {
  void* val;
  do {
    queue_dequeue (q, &val, NULL, NULL);
  } while (val);
  for (struct queue_e* e = q->free; e; e = e->next)
    free (e);
  free (q);
}

void queue_enqueue (queue_t q, void* val, void* arg, void (*callback) (void*, void*)) {
  spinlock_lock (&q->mutex);
    struct queue_e* qe;
    if (q->free != NULL) {
      qe      = q->free;
      q->free = q->free->next;
    } else
      qe = malloc (sizeof (struct queue_e));
    qe->val = val;
    qe->arg = arg;
    qe->callback = callback;
    qe->prev = NULL;
    qe->next = q->back;
    if (q->back != NULL)
      q->back->prev = qe;
    else
      q->front = qe;
    q->back = qe;
  spinlock_unlock (&q->mutex);
}

void queue_dequeue (queue_t q, void** val, void ** arg, void (**callback) (void*, void*)) {
  spinlock_lock (&q->mutex);
    if (q->front != NULL) {
      *val = q->front->val;
      if (arg)
        *arg = q->front->arg;
      if (callback)
        *callback = q->front->callback;
      struct queue_e* new_front = q->front->prev;
      q->front->next = q->free;
      q->free        = q->front;
      if (new_front != NULL)
        new_front->next = NULL;
      else
        q->back = NULL;
      q->front = new_front;
    } else {
      *val = NULL;
      if (callback)
        *callback = NULL;
    }
  spinlock_unlock (&q->mutex);
}
