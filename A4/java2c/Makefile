CFLAGS += -std=gnu11 -g
EXES    = BinaryTree

all:  $(EXES)
clean:
	-rm $(EXES)
	-rm -r BinaryTree.dSYM

namelist: BinaryTree.c
# don't treat all and clean as file targets
.PHONY: all clean
