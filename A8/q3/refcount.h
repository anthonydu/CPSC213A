#pragma once

typedef void (*rc_finalizer_t)(void *);
void* rc_malloc   (int nbytes, rc_finalizer_t finalizer);
void  rc_keep_ref (void* p);
void  rc_free_ref (void* p);
