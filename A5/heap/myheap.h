#ifndef _MYHEAP_H_
#define _MYHEAP_H_

/*
 * Struct used to represent the heap.
 */
struct myheap;

/*
 * Create a heap that is "size" bytes large.
 */
struct myheap *heap_create(unsigned int size);

/*
 * Free a block on the heap h.
 */
void myheap_free(struct myheap *h, void *payload);

/*
 * Our implementation of malloc.
 */
void *myheap_malloc(struct myheap *h, unsigned int size);

#endif
