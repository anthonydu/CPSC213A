CFLAGS += -std=gnu11 -g -I..
MAIN_EXE = main
MAIN_OBJ = main.o list.o tree.o str_element.o int_element.o ../refcount.o
SORT_EXE = sortmain
SORT_OBJ = sortmain.o str_element.o int_element.o ../refcount.o

EXES = $(MAIN_EXE) $(SORT_EXE)
OBJS = $(MAIN_OBJ) $(SORT_OBJ)

all: $(EXES)

$(MAIN_EXE): $(MAIN_OBJ)
$(SORT_EXE): $(SORT_OBJ)

clean:
	rm -f $(EXES) $(OBJS)
tidy:
	rm -f $(OBJS)
