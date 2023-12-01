#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <assert.h>
#include "uthread.h"
#include "disk.h"

unsigned int sum = 0;
volatile int is_read_pending;  // flag that indicates whether a read is pending
                               // set before requesting disk_read
                               // cleared by interrupt service routine

/**
 * Called by disk subsystem each time that a read completes.
 * You may assume that reads complete in request order.
 */
void interrupt_service_routine() {
  is_read_pending = 0; // clear flag to indicate that read is no longer pending
}

int main (int argc, char** argv) {

  // Read Command Line Arguments
  static char* usage = "usage: sRead num_blocks";
  int num_blocks;
  char *endptr;
  if (argc == 2)
    num_blocks = strtol (argv [1], &endptr, 10);
  if (argc != 2 || *endptr != 0) {
    printf ("argument error - %s \n", usage);
    return EXIT_FAILURE;
  }

  // Initialize
  uthread_init (1);
  disk_start (interrupt_service_routine);

  // Sum Blocks
  for (int blockno = 0; blockno < num_blocks; blockno++) {
    int result;
    is_read_pending = 1;                     // set flag saying that read will be pending
    disk_schedule_read (&result, blockno);   // request disk to read specified blockno
    while (is_read_pending);                 // loop until is_read_pending == 0
    sum += result;
  }
  printf ("%d\n", sum);
}
