//
// Written by Mike Feeley, University of BC, 2010-2014
// Do not redistribute any portion of this code without permission.
//

#ifndef __spinlock_h__
#define __spinlock_h__

typedef volatile int spinlock_t;

/* Initialize a spinlock. Spinlocks hold all state in the spinlock_t, so they don't need to be destroyed or freed. */
void       spinlock_create (spinlock_t* lock);

/* Lock the spinlock, spinning the CPU until the lock is available. */
void       spinlock_lock   (spinlock_t* lock);

/* Unlock the spinlock. This completes immediately without spinning. */
void       spinlock_unlock (spinlock_t* lock);

#endif
