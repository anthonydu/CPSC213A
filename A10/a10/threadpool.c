#include "threadpool.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "spinlock.h"
#include "uthread.h"
#include "uthread_mutex_cond.h"

struct queue {
  struct queue_e *front, *back, *free;
  spinlock_t mutex;
};
typedef struct queue* queue_t;

struct queue_e {
  void *val, *arg;
  void (*callback)(void*, void*);
  struct queue_e *prev, *next;
};

queue_t queue_create() {
  queue_t q = malloc(sizeof(struct queue));
  q->front = NULL;
  q->back = NULL;
  q->free = NULL;
  spinlock_create(&q->mutex);
  return q;
}

void queue_destroy(queue_t q) {
  void* val;
  do {
    queue_dequeue(q, &val, NULL, NULL);
  } while (val);
  for (struct queue_e* e = q->free; e; e = e->next) free(e);
  free(q);
}

void queue_enqueue(queue_t q,
                   void* val,
                   void* arg,
                   void (*callback)(void*, void*)) {
  spinlock_lock(&q->mutex);
  struct queue_e* qe;
  if (q->free != NULL) {
    qe = q->free;
    q->free = q->free->next;
  } else
    qe = malloc(sizeof(struct queue_e));
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
  spinlock_unlock(&q->mutex);
}

void queue_dequeue(queue_t q,
                   void** val,
                   void** arg,
                   void (**callback)(void*, void*)) {
  spinlock_lock(&q->mutex);
  if (q->front != NULL) {
    *val = q->front->val;
    if (arg) *arg = q->front->arg;
    if (callback) *callback = q->front->callback;
    struct queue_e* new_front = q->front->prev;
    q->front->next = q->free;
    q->free = q->front;
    if (new_front != NULL)
      new_front->next = NULL;
    else
      q->back = NULL;
    q->front = new_front;
  } else {
    *val = NULL;
    if (callback) *callback = NULL;
  }
  spinlock_unlock(&q->mutex);
}

struct tpool {
  unsigned int max_threads;
  queue_t pending_tasks;
  unsigned int num_pending;
  uthread_t* worker_threads;
  uthread_mutex_t mutex;
  uthread_cond_t cond;
};

/**
 * Base procedure of every worker thread.  Calls available tasks
 * or blocks until a task becomes available.
 */
void* worker_thread(void* pool_v) {
  tpool_t pool = pool_v;

  // TODO
  while (1) {
    uthread_mutex_lock(pool->mutex);
    while (!pool->num_pending) uthread_cond_wait(pool->cond);
    void* val;
    void* arg;
    void (*f)(tpool_t, void*);
    queue_dequeue(pool->pending_tasks, &val, &arg, &f);
    pool->num_pending--;
    uthread_mutex_unlock(pool->mutex);
    f(val, arg);
  }
  return NULL;  // TODO: replace this placehold return value
}

/**
 * Create a new thread pool with max_threads thread-count limit.
 */
tpool_t tpool_create(unsigned int max_threads) {
  tpool_t pool = malloc(sizeof(struct tpool));
  uthread_mutex_t mutex = uthread_mutex_create();
  uthread_cond_t cond = uthread_cond_create(mutex);
  uthread_t* worker_threads = malloc(sizeof(uthread_t) * max_threads);
  pool->max_threads = max_threads;
  pool->pending_tasks = queue_create();
  pool->num_pending = 0;
  pool->worker_threads = worker_threads;
  pool->mutex = mutex;
  pool->cond = cond;
  for (int i = 0; i < max_threads; i++) {
    worker_threads[i] = uthread_create(worker_thread, pool);
  }
  return pool;
}

/**
 * Sechedule task f(arg) to be executed.
 */
void tpool_schedule_task(tpool_t pool, void (*f)(tpool_t, void*), void* arg) {
  uthread_mutex_lock(pool->mutex);
  queue_enqueue(pool->pending_tasks, pool, arg, f);
  pool->num_pending++;
  uthread_cond_signal(pool->cond);
  uthread_mutex_unlock(pool->mutex);
}

/**
 * Wait (by blocking) until all tasks have completed and thread pool is thus
 * idle
 */
void tpool_join(tpool_t pool) {
  uthread_mutex_lock(pool->mutex);
  while (pool->num_pending) uthread_cond_wait(pool->cond);
  uthread_mutex_unlock(pool->mutex);
}