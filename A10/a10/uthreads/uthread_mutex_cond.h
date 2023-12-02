//
// Written by Mike Feeley, University of BC, 2010-2014
// Do not redistribute any portion of this code without permission.
//

#ifndef __uthread_mutex_cond_h__
#define __uthread_mutex_cond_h__

struct uthread_mutex;
typedef struct uthread_mutex* uthread_mutex_t;
struct uthread_cond;
typedef struct uthread_cond*  uthread_cond_t;

/* Create a new mutex object. Mutexes are initially unlocked. */
uthread_mutex_t uthread_mutex_create        (void);

/* Lock a mutex for exclusive access. This will block until the mutex is available.
Only one thread may hold the lock at any given time. */
void            uthread_mutex_lock          (uthread_mutex_t);

/* Lock a mutex for shared, read-only access. This will block until the mutex is available.
Any number of threads may hold a read-only lock simultaneously as long as no thread
holds an exclusive lock. This is intended to allow simultaneous read-only access to shared data. */
void            uthread_mutex_lock_readonly (uthread_mutex_t);

/* Unlock a mutex (either readonly or exclusive), relinquishing this thread's lock.
This signal waiters as appropriate and never blocks. */
void            uthread_mutex_unlock        (uthread_mutex_t);

/* Destroy a mutex. Only do this if nothing is waiting or using the mutex. */
void            uthread_mutex_destroy       (uthread_mutex_t);


/* Create a new condition variable, bound to a particular mutex. */
uthread_cond_t  uthread_cond_create         (uthread_mutex_t);

/* Wait for a condition variable to be signalled. This will *always* block!
The associated mutex must be locked by the current thread, and will be
unlocked when this thread blocks. */
void            uthread_cond_wait           (uthread_cond_t);

/* Signal the condition variable, waking up to *one* waiting thread.
The associated mutex must be locked by the current thread.
This will never block. */
void            uthread_cond_signal         (uthread_cond_t);

/* Broadcast to the condition variable, waking up *all* waiting threads.
The associated mutex must be locked by the current thread.
This will never block. */
void            uthread_cond_broadcast      (uthread_cond_t);

/* Destroy a condition variable. Only do this if nothing is waiting on it! */
void            uthread_cond_destroy        (uthread_cond_t);

#endif
