#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/errno.h>

#include "disk.h"
#include "queue.h"
#include "uthread.h"

queue_t pending_read_queue;

void interrupt_service_routine() {
  void *val;
  void *arg;
  void (*callback)(void *, void *);
  queue_dequeue(pending_read_queue, &val, &arg, &callback);
  callback(val, arg);
}

void handleOtherReads(void *resultv, void *countv) {
  (*(int *)countv)--;
  disk_schedule_read(resultv, *(int *)resultv);
  queue_enqueue(pending_read_queue, resultv, countv, handleOtherReads);
}

void handleFirstRead(void *resultv, void *countv) {
  *(int *)countv = *(int *)resultv;
  handleOtherReads(resultv, countv);
}

int main(int argc, char **argv) {
  // Command Line Arguments
  static char *usage = "usage: treasureHunt starting_block_number";
  int starting_block_number;
  char *endptr;
  if (argc == 2) starting_block_number = strtol(argv[1], &endptr, 10);
  if (argc != 2 || *endptr != 0) {
    printf("argument error - %s \n", usage);
    return EXIT_FAILURE;
  }

  // Initialize
  uthread_init(1);
  disk_start(interrupt_service_routine);
  pending_read_queue = queue_create();

  // Start the Hunt
  int result, count = 1;
  disk_schedule_read(&result, starting_block_number);
  queue_enqueue(pending_read_queue, &result, &count, handleFirstRead);

  while (count >= 0)
    ;  // infinite loop so that main doesn't return before hunt completes

  printf("%d\n", result);
}
