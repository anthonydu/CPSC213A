#ifndef __uthread_h__
#define __uthread_h__

/* Basic definitions for the uthread_t type. (Ignore uthread_TCB, that's an implementation detail) */
struct uthread_TCB;
typedef struct uthread_TCB* uthread_t;

/* Initialize the uthread library. You must call this exactly once before
calling any other uthread function.
num_processors specifies the number of virtual cores in the CPU (number of
simultaneously-executing uthreads). */
void      uthread_init    (int num_processors);

/* Create a uthread. The created uthread will call start_proc(start_arg), and
its return value will be made available via uthread_join. */
uthread_t uthread_create  (void* (*start_proc)(void*), void* start_arg);

void      uthread_detach  (uthread_t thread);

/* Join a uthread. This blocks the calling thread until the target thread
exits, and then returns the target thread's return value in *value_ptr. */
int       uthread_join    (uthread_t thread, void** value_ptr);

/* Get the uthread handle for the currently-executing thread. */
uthread_t uthread_self();

/* Yield execution. This gives up the CPU to another thread. */
void      uthread_yield();

/* Block the calling thread. The thread can only be unblocked by a call
to uthread_unblock from a different thread. */
void      uthread_block();

/* Unblock another thread, allowing it to run again.
Note: if the target thread is not currently blocked, the target's next
call to uthread_block will return immediately, effectively unblocking it. */
void      uthread_unblock (uthread_t thread);

#endif
