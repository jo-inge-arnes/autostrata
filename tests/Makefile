IDIR=../include
CC=gcc
CFLAGS=-Wall -I$(IDIR)
LDIR =../lib
LIBS=-lautostrata -lm
_DEPS=autostrata.h structs.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS)) tests.h structutilstests.h initializationtests.h
TESTSOBJ=tests.o structutilstests.o initializationtests.o

%.o: %.c $(DEPS)
	$(CC) $(DEBUG) -c -o $@ $< $(CFLAGS)

tests: $(TESTSOBJ)
	$(CC) $(DEBUG) -o $@ $^ -L$(LDIR) $(LIBS)

.PHONY: all
all: tests

.PHONY: clean
clean:
	rm -rf *.o

.PHONY: debug
debug:
	$(MAKE) $(MAKEFILE) DEBUG="-g"

.PHONY: valgrind
valgrind: $(DEPS) $(TESTSOBJ)
	@valgrind --leak-check=full -v ./tests
