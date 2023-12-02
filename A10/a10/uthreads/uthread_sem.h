//
// Written by Mike Feeley, University of BC, 2010-2014
// Do not redistribute any portion of this code without permission.
//

#ifndef __uthread_sem_h__
#define __uthread_sem_h__

struct uthread_sem;
typedef struct uthread_sem* uthread_sem_t;

/* Create a semaphore object that holds a particular initial value.
All semaphores are unbounded (can hold any non-negative value). */
uthread_sem_t uthread_sem_create  (int initial_value);

/* Destroy a semaphore. Only call this if nothing is blocked on the semaphore! */
void          uthread_sem_destroy (uthread_sem_t);

/* Wait for a semaphore to become non-zero, then decrement it.
If the semaphore is zero on entry, block until another thread signals this semaphore. */
void          uthread_sem_wait    (uthread_sem_t);

/* Increment the semaphore, and signal a single waiting thread. This never blocks. */
void          uthread_sem_signal  (uthread_sem_t);

#endif
