#include <stdlib.h>
#include "refcount.h"

/* rc_header (refcount metadata) is prepended to the client's area.

On 32-bit machines, this is conveniently 8 bytes long.
On 64-bit machines, this is 16 bytes long, which is still aligned to 8 bytes.
*/
struct rc_header {
  int refcount;
  rc_finalizer_t finalizer;
};

void *rc_malloc(int nbytes, rc_finalizer_t finalizer) {
  // allocate enough room for metadata
  struct rc_header *rc = malloc(nbytes + sizeof(struct rc_header));
  // initialize metadata
  rc->refcount = 1;
  rc->finalizer = finalizer;
  // return offset pointer (+1 on a structure type points just past the end)
  return (void *)(rc + 1);
}

void rc_keep_ref(void *p) {
  // get pointer to refcount metadata from client pointer
  struct rc_header *rc = ((struct rc_header *)p) - 1;
  // increment refcount
  rc->refcount++;
}

void rc_free_ref(void *p) {
  // get pointer to refcount metadata from client pointer
  struct rc_header *rc = ((struct rc_header *)p) - 1;
  // decrement refcount
  rc->refcount--;
  // if our refcount is now zero, cleanup the object
  if(rc->refcount == 0) {
    if(rc->finalizer)
      rc->finalizer(p);
    free(rc);
  }
}
