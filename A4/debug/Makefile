CFLAGS += -std=gnu11 -g
EXES    = namelist

all:  $(EXES)
clean:
	-rm $(EXES)
	-rm -r namelist.dSYM

namelist: namelist.c
# don't treat all and clean as file targets
.PHONY: all clean
