CFLAGS  += -std=gnu11 -g
UNAME = $(shell uname)
ifeq ($(UNAME), Linux)
LDFLAGS += -pthread
endif
EXES = tRead

all: $(EXES)

clean:
	rm -f *.o $(EXES); rm -rf *.dSYM
tidy:
	rm -f *.o; rm -rf *.dSYM

tRead: tRead.o disk.o uthread.o queue.o

