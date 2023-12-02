#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "uthread.h"
#include "threadpool.h"

void my_task(tpool_t pool, void *arg) {
  long i = (long) arg;
  printf("Task %ld started\n", i);
  usleep(1000000);
  printf("Task %ld ended\n", i);
}

int main(int argc, char *argv[]) {

  tpool_t pool;
  int num_threads, num_tasks;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s NUM_THREADS NUM_TASKS\n", argv[0]);
    return -1;
  }
  
  num_threads = strtol(argv[1], NULL, 10);
  num_tasks = strtol(argv[2], NULL, 10);
  
  uthread_init(8);
  pool = tpool_create(num_threads);

  for (long i = 0; i < num_tasks; i++)
    tpool_schedule_task(pool, my_task, (void *) i);

  tpool_join(pool);
  
  return 0;
}
