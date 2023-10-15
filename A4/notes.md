# Notes

```c
// imports from /usr/include
#include <stdlib.h> // malloc, etc.
#include <stdio.h> // printf, etc.
#include "" // import local headers
```

## Compilation

```shell
gcc -std=gnu11 -g -o example example.c
# -std=gnu11: use gnu11 standard
# -g: adds debugging information
# -o: output file
```

## Debugging a C program

```shell
gdb foo # For programs compiled with gcc
lldb foo # For programs compiled with Clang
```

### Example commands

- `b <location>`: set breakpoint
- `run [arguments]`: run a program
- `bt`: print out backtrace
- `p <variable>`: print out current value of a variable
- `s` (step): step into function call
- `n` (next): step to the next line of code
- `cont`: continue execution
- `help`: get a list of available commands

## Makefile

It's like package.json for C.

```makefile
# variables
CC = gcc # compiler
CFLAGS += -std=gnu11 -g # compiler flags
EXE = exe # list of executables
OBJS = obj1.o obj2.o obj3.o # list of objects

# targets
# leading tabs are required
# @ commands are not printed to stdout

# first target is default target
# compile all objects into exe
all: $(OBJS)
	@echo "Compiling all executables into exe..."
	$(CC) $(CFLAGS) $(OBJS) -o out
	@echo "Done!"

# compile src1.c into obj1
obj1.o: src1.c
	@echo "Compiling src1.c into obj1..."
	$(CC) $(CFLAGS) -c src1.c
	@echo "Done!"

# compile src2.c into obj2
obj2.o: src2.c
	@echo "Compiling src2.c into obj2..."
	$(CC) $(CFLAGS) -c src2.c
	@echo "Done!"

# compile sec3.c into obj3
obj3.o: src3.c
	@echo "Compiling src3.c into obj3..."
	$(CC) $(CFLAGS) -c src3.c
	@echo "Done!"

# make clean
clean:
	@echo "Removing all objects and executables..."
	rm $(OBJS) $(EXE)
	@echo "Done!"

# do not treat `clean` as file target
.PHONY: clean
```
