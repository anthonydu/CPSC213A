#pragma once

typedef struct tpool *tpool_t;

tpool_t tpool_create(unsigned int max_threads);
void tpool_schedule_task(tpool_t pool, void (*f)(tpool_t, void *), void *arg);
void tpool_join(tpool_t pool);
