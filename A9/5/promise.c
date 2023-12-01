#include <stdlib.h>
#include "promise.h"

struct promise {
  int       is_resolved;
  void*     value;
  void*     (^on_fulfilled) (void*);
  promise_t fulfilled_promise, parent;
};

promise_t promise_new (promise_t parent) {
  promise_t p          = malloc (sizeof (struct promise));
  p->is_resolved       = 0;
  p->value             = NULL;
  p->on_fulfilled      = NULL;
  p->fulfilled_promise = NULL;
  p->parent            = parent;
  return p;
}

void promise_free (promise_t p) {
  if (p->parent)
    promise_free (p->parent);
  free (p);
}

void promise_resolve (promise_t p, void* value) {
  p->is_resolved = 1;
  p->value       = value;
  if (p->on_fulfilled)
    promise_resolve (p->fulfilled_promise, p->on_fulfilled (value));
}

promise_t promise_then (promise_t p, void* (^on_fulfilled)(void*)) {
  promise_t tp = promise_new (p);
  if (p->is_resolved)
    promise_resolve (tp, on_fulfilled (p->value));
  else {
    p->on_fulfilled      = on_fulfilled;
    p->fulfilled_promise = tp;
  }
  return tp;
}
