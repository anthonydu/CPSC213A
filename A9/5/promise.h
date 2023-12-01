#ifndef __promise_h__
#define __promise_h__

struct promise;
typedef struct promise* promise_t;

promise_t promise_new     (promise_t parent);
void      promise_free    (promise_t p);
void      promise_resolve (promise_t p, void* value);
promise_t promise_then    (promise_t p, void* (^on_fulfilled) (void*));

#endif