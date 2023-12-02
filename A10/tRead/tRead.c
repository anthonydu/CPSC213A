#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

#include "disk.h"
#include "queue.h"
#include "uthread.h"

queue_t pending_read_queue;
unsigned int sum = 0;

void interrupt_service_routine() {
  // TODO
  uthread_t thread;
  queue_dequeue(pending_read_queue, &thread, NULL, NULL);
  uthread_unblock(thread);
}

void* read_block(void* blocknov) {
  // TODO schedule read and the update (correctly)
  int result;
  queue_enqueue(pending_read_queue, uthread_self(), NULL, NULL);
  disk_schedule_read(&result, blocknov);
  uthread_block();
  sum += result;
}

int main(int argc, char** argv) {
  // Command Line Arguments
  static char* usage = "usage: tRead num_blocks";
  int num_blocks;
  char* endptr;
  if (argc == 2) num_blocks = strtol(argv[1], &endptr, 10);
  if (argc != 2 || *endptr != 0) {
    printf("argument error - %s \n", usage);
    return EXIT_FAILURE;
  }

  // Initialize
  uthread_init(1);
  disk_start(interrupt_service_routine);
  pending_read_queue = queue_create();

  // Sum Blocks
  // TODO
  uthread_t threads[num_blocks];
  for (int blockno = 0; blockno < num_blocks; blockno++) {
    threads[blockno] = uthread_create(&read_block, blockno);
  }
  // joins main to all threads to prevent return
  for (int blockno = 0; blockno < num_blocks; blockno++) {
    uthread_join(threads[blockno], NULL);
  }

  printf("%d\n", sum);
}
