UTHREAD = ./uthreads
TARGETS = q1 q2 q3 q4 smoke use_threadpool traffic

OBJS = $(UTHREAD)/uthread.o $(UTHREAD)/uthread_mutex_cond.o $(UTHREAD)/uthread_sem.o
JUNK = $(OBJS) *.o
CFLAGS  += -g -std=gnu11 -I$(UTHREAD)
UNAME = $(shell uname)
ifeq ($(UNAME), Linux)
LDFLAGS += -pthread
endif

all: $(TARGETS)
$(TARGETS): $(OBJS)

use_threadpool: use_threadpool.c threadpool.c

clean:
	-rm -f $(JUNK) $(TARGETS)
tidy: clean
	rm -rf $(JUNK)


