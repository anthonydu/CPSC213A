#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "myheap.h"


/* This is the testing harness for myheap_malloc */

#define MB (1024 * 1024)

struct testfunc;
typedef int (*testfunc_t)(struct testfunc *tf);

struct testfunc {
  const char *name;
  int heapsize;
  testfunc_t func;
  struct myheap *heap; // filled in by test routine
};

/* Check a new allocation to see if it's legal, and to see if it overlaps with any existing allocations */
static int check_alloc(struct testfunc *tf, void *ptr, int size, void **ptrs, int *sizes, int nptrs) {
  if(ptr == NULL) {
    fprintf(stderr, "%s: myheap_malloc returned NULL (maybe it ran out of memory)\n", tf->name);
    return -1;
  }
  if(((uintptr_t)ptr) % sizeof(int) != 0) {
    fprintf(stderr, "%s: myheap_malloc returned an unaligned pointer %p\n", tf->name, ptr);
    return -1;
  }
  if(ptr < (void *) tf->heap ||
     ptr + size > (void *) tf->heap + tf->heapsize) {
    fprintf(stderr, "%s: myheap_malloc returned invalid pointer: %p is out of the bounds of the heap [%p, %p]\n",
      tf->name, ptr, tf->heap, ((void*) tf->heap) + tf->heapsize);
    return -1;
  }
  for(int i=0; i<nptrs; i++) {
    if(ptr >= ptrs[i] && ptr < ptrs[i] + sizes[i]) {
      fprintf(stderr, "%s: myheap_malloc returned an overlapping pointer: %p lies inside existing allocation [%p, %p]\n",
        tf->name, ptr, ptrs[i], ptrs[i] + sizes[i]);
      return -1;
    }
  }
  /* Fill the new allocation with stuff - ensuring that malloc is
     letting us use the whole space.  If you see crashes from
     addresses like "0xcccccccc", this line probably overwrote a
     pointer which was improperly stored inside the allocated area. */
  memset(ptr, 0xcc, size);
  return 0;
}

#define SANITY_SIZE 16
int test_sanity(struct testfunc *tf) {
  /* The most basic sanity test - check that malloc and free work once. */
  void *ptr = myheap_malloc(tf->heap, SANITY_SIZE);
  if(0 != check_alloc(tf, ptr, SANITY_SIZE, 0, 0, 0)) {
    return -1;
  }
  myheap_free(tf->heap, ptr);
  return 0;
}

#define FREELIST1_SIZE 16
int test_freelist1(struct testfunc *tf) {
  /* Check that the implementation is using a free list. */

  /* Allocate a pointer and then free it */
  void *ptr = myheap_malloc(tf->heap, FREELIST1_SIZE);
  if(0 != check_alloc(tf, ptr, FREELIST1_SIZE, 0, 0, 0)) {
    return -1;
  }
  myheap_free(tf->heap, ptr);
  /* Allocate the same size again. With a free-list, we expect to get the same pointer back. */
  void *ptr2 = myheap_malloc(tf->heap, FREELIST1_SIZE);
  if(0 != check_alloc(tf, ptr2, FREELIST1_SIZE, 0, 0, 0)) {
    return -1;
  }
  if(ptr != ptr2) {
    fprintf(stderr, "%s: myheap_malloc did not reuse free chunk %p (returned %p instead)\n",
      tf->name, ptr, ptr2);
    return -1;
  }
  return 0;
}

#define FREELIST2_SIZE 16
int test_freelist2(struct testfunc *tf) {
  /* Check that the implementation is using a free list. */
  void *p[2] = {0};
  void *q[2] = {0};
  int sizes[2] = {0};

  /* Allocate two chunks of memory, then free them. */
  for(int i=0; i<2; i++) {
    void *ptr = myheap_malloc(tf->heap, FREELIST2_SIZE);
    if(0 != check_alloc(tf, ptr, FREELIST2_SIZE, p, sizes, 2)) {
      return -1;
    }
    p[i] = ptr;
    sizes[i] = FREELIST2_SIZE;
  }
  myheap_free(tf->heap, p[0]);
  myheap_free(tf->heap, p[1]);

  /* Verify that a free-list based allocator reuses the two chunks above. */
  for(int i=0; i<2; i++) {
    void *ptr = myheap_malloc(tf->heap, FREELIST2_SIZE);
    if(0 != check_alloc(tf, ptr, FREELIST2_SIZE, q, sizes, 2)) {
      return -1;
    }
    if(ptr != p[0] && ptr != p[1]) {
      fprintf(stderr, "%s: myheap_malloc did not reuse either free chunk %p or %p (returned %p instead)\n",
        tf->name, p[0], p[1], ptr);
      return -1;
    }
    q[i] = ptr;
    sizes[i] = FREELIST2_SIZE;
  }
  myheap_free(tf->heap, q[0]);
  myheap_free(tf->heap, q[1]);

  return 0;
}

#define SIMPLE_NBUCKETS 4
#define SIMPLE_ITERS 16
int test_simple(struct testfunc *tf) {
  /* A relatively simple test which allocates and frees a few items repeatedly. */
  int sizes[SIMPLE_NBUCKETS];
  for(int i=0; i<SIMPLE_NBUCKETS; i++) {
    sizes[i] = i * 8;
  }

  void *ptrs[SIMPLE_NBUCKETS] = {0};
  for(int i=0; i<SIMPLE_ITERS; i++) {
    int idx = i % SIMPLE_NBUCKETS;
    if(ptrs[idx]) {
      myheap_free(tf->heap, ptrs[idx]);
      ptrs[idx] = NULL;
    } else {
      void *ptr = myheap_malloc(tf->heap, sizes[idx]);
      if(0 != check_alloc(tf, ptr, sizes[idx], ptrs, sizes, SIMPLE_NBUCKETS)) {
        return -1;
      }
      ptrs[idx] = ptr;
    }
  }
  return 0;
}

#define FIXED_NBUCKETS 128
#define FIXED_ITERS 16384
#define FIXED_SIZE 256
int test_fixedsize(struct testfunc *tf) {
  /* A harder test which allocates and frees a lot of equal-sized elements many times */
  int sizes[FIXED_NBUCKETS];
  for(int i=0; i<FIXED_NBUCKETS; i++) {
    sizes[i] = FIXED_SIZE;
  }

  void *ptrs[FIXED_NBUCKETS] = {0};
  for(int i=0; i<FIXED_ITERS; i++) {
    int idx = i % FIXED_NBUCKETS;
    if(ptrs[idx]) {
      myheap_free(tf->heap, ptrs[idx]);
      ptrs[idx] = NULL;
    } else {
      void *ptr = myheap_malloc(tf->heap, sizes[idx]);
      if(0 != check_alloc(tf, ptr, sizes[idx], ptrs, sizes, FIXED_NBUCKETS)) {
        return -1;
      }
      ptrs[idx] = ptr;
    }
  }
  return 0;
}

#define RAND_NBUCKETS 512
#define RAND_ITERS 102400
#define RAND_MINSIZE 64
#define RAND_MAXSIZE 4096
int test_random(struct testfunc *tf) {
  /* Allocate/free a whole bunch of memory at random.
     At any given time, the total usage is bounded above by NBUCKETS * MAXSIZE,
     so a good free-list implementation should never run out of space.
  */
  srand(0x31337);
  int sizes[RAND_NBUCKETS];
  for(int i=0; i<RAND_NBUCKETS; i++) {
    sizes[i] = rand() % (RAND_MAXSIZE - RAND_MINSIZE) + RAND_MINSIZE;
  }

  void *ptrs[RAND_NBUCKETS] = {0};
  for(int i=0; i<RAND_ITERS; i++) {
    int idx = rand() % RAND_NBUCKETS;
    if(ptrs[idx]) {
      myheap_free(tf->heap, ptrs[idx]);
      ptrs[idx] = NULL;
    } else {
      void *ptr = myheap_malloc(tf->heap, sizes[idx]);
      if(0 != check_alloc(tf, ptr, sizes[idx], ptrs, sizes, RAND_NBUCKETS)) {
        return -1;
      }
      ptrs[idx] = ptr;
    }
  }
  return 0;
}

#define COALESCE1_SIZE (333 * 1024)
#define COALESCE1_COUNT 3

int test_coalesce1(struct testfunc *tf) {
  /* Simple test to exercise coalescing and splitting */
  /* step 1: allocate three big chunks to fill the heap */
  int sizes[COALESCE1_COUNT] = {0};
  void *ptrs[COALESCE1_COUNT] = {0};
  for(int i=0; i<COALESCE1_COUNT; i++) {
    sizes[i] = COALESCE1_SIZE;
    void *ptr = myheap_malloc(tf->heap, sizes[i]);
    if(0 != check_alloc(tf, ptr, sizes[i], ptrs, sizes, COALESCE1_COUNT)) {
      return -1;
    }
    ptrs[i] = ptr;
  }

  /* step 2: free two adjacent chunks and allocate them as one block */
  myheap_free(tf->heap, ptrs[1]);
  ptrs[1] = NULL;
  myheap_free(tf->heap, ptrs[2]);
  ptrs[2] = NULL;

  int size1 = COALESCE1_SIZE * 2;
  void *ptr1 = myheap_malloc(tf->heap, size1);
  if(0 != check_alloc(tf, ptr1, size1, ptrs, sizes, COALESCE1_COUNT)) {
    return -1;
  }

  /* step 3: free two more chunks and allocate them as one block */
  myheap_free(tf->heap, ptr1);
  ptr1 = NULL;
  myheap_free(tf->heap, ptrs[0]);
  ptrs[0] = NULL;

  int size2 = COALESCE1_SIZE * 3;
  void *ptr2 = myheap_malloc(tf->heap, size2);
  if(0 != check_alloc(tf, ptr2, size2, ptrs, sizes, COALESCE1_COUNT)) {
    return -1;
  }
  myheap_free(tf->heap, ptr2);
  ptr2 = NULL;

  return 0;
}

#define COALESCE2_COUNT 960
#define COALESCE2_SIZE 1024
#define COALESCE2_BIGSIZE (960 * 1024)

int test_coalesce2(struct testfunc *tf) {
  /* Allocate a lot of small chunks, free them all, and allocate one big check.
     Exercises the coalesce() implementation.
  */
  srand(0x1337);

  int sizes[COALESCE2_COUNT] = {0};
  for(int i=0; i<COALESCE2_COUNT; i++) {
    sizes[i] = COALESCE2_SIZE;
  }

  void *ptrs[COALESCE2_COUNT] = {0};
  for(int i=0; i<COALESCE2_COUNT; i++) {
    void *ptr = myheap_malloc(tf->heap, sizes[i]);
    if(0 != check_alloc(tf, ptr, sizes[i], ptrs, sizes, COALESCE2_COUNT)) {
      return -1;
    }
    ptrs[i] = ptr;
  }

  /* Shuffle the pointers array so we free all the chunks in a random order */
  for(int i=COALESCE2_COUNT - 1; i>=1; i--) {
    int j = rand() % (i+1);
    void *ptr = ptrs[i];
    ptrs[i] = ptrs[j];
    ptrs[j] = ptr;
  }

  for(int i=0; i<COALESCE2_COUNT; i++) {
    myheap_free(tf->heap, ptrs[i]);
    ptrs[i] = NULL;
  }

  void *ptr = myheap_malloc(tf->heap, COALESCE2_BIGSIZE);
  if(0 != check_alloc(tf, ptr, COALESCE2_BIGSIZE, 0, 0, 0)) {
    return -1;
  }
  return 0;
}

static struct testfunc testfuncs[] = {
  { "sanity", 1 * MB, test_sanity },
  { "freelist1", 1 * MB, test_freelist1 },
  { "freelist2", 1 * MB, test_freelist2 },
  { "simple", 1 * MB, test_simple },
  { "fixedsize", 1 * MB, test_fixedsize },
  { "random", 8 * MB, test_random },
  { "coalesce1", 1 * MB, test_coalesce1 },
  { "coalesce2", 1 * MB, test_coalesce2 },
  { NULL, 0, NULL },
};

int test(struct testfunc *tf) {
  /* Create heap for myheap_malloc to use */
  tf->heap = heap_create(tf->heapsize);
  if(tf->heap == NULL) {
    fprintf(stderr, "test %s: failed to allocate heap!\n", tf->name);
    return -1;
  }
  int result = tf->func(tf);
  tf->heap = NULL;

  if(result != 0) {
    fprintf(stderr, "test %s failed\n", tf->name);
  } else {
    printf("test %s passed!\n", tf->name);
  }
  return result;
}

void usage(char *progname) {
  fprintf(stderr, "Usage: %s <test|all>\n", progname);
  fprintf(stderr, "Available tests (all = run all tests sequentially):\n");
  for(struct testfunc *tf = &testfuncs[0]; tf->name; tf++) {
    fprintf(stderr, "  %s\n", tf->name);
  }
  exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
  if(argc != 2) {
    usage(argv[0]);
  }

  char *testname = argv[1];
  if(0 == strcmp(testname, "all")) {
    int result = 0;
    for(struct testfunc *tf = &testfuncs[0]; tf->name; tf++) {
      if(0 != test(tf)) {
        result = -1;
      }
    }
    if(result == 0) {
      printf("All tests passed!\n");
      exit(EXIT_SUCCESS);
    } else {
      fprintf(stderr, "Some tests failed.\n");
      exit(EXIT_FAILURE);
    }
  }
  for(struct testfunc *tf = &testfuncs[0]; tf->name; tf++) {
    if(strcmp(testname, tf->name) == 0) {
      if(0 == test(tf)) {
        exit(EXIT_SUCCESS);
      } else {
        exit(EXIT_FAILURE);
      }
    }
  }
  usage(argv[0]);
}
