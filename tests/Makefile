IDIR=../include
CC=gcc
CFLAGS=-I$(IDIR)
LDIR =../lib
LIBS=-lautostrata
_DEPS=autostrata.h structs.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))
TESTSOBJ=tests.o

%.o: %.c tests.h $(DEPS)
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
