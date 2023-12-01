#ifndef __queue_h__
#define __queue_h__

struct queue;
typedef struct queue* queue_t;

queue_t queue_create  ();

/**
 * Enqueue the tuple (val, arg, callback)
 */
void queue_enqueue (queue_t q, void* val, void* arg,   void (*callback)  (void*, void*));

/**
 * Delete and place results into *val, *arg, and *callback
 * Note that arg is optional (and only used in treasureHunt); if arg == NULL then it is ignored
 */
void queue_dequeue (queue_t q, void **val, void **arg, void (**callback) (void*, void*));

#endif
