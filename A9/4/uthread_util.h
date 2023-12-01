#ifndef __uthread_util_h__
#define __uthread_util_h__

struct uthread_queue {
  uthread_t head, tail;
};
typedef struct uthread_queue uthread_queue_t;

void      uthread_initqueue      (uthread_queue_t*);
void      uthread_enqueue        (uthread_queue_t*, uthread_t);
uthread_t uthread_dequeue        (uthread_queue_t*);
int       uthread_queue_is_empty (uthread_queue_t* queue);

void uthread_setInterrupt (int);

#endif
