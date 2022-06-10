IDIR=../include
CC=gcc
CFLAGS=-I$(IDIR)
LIBS=
ODIR=obj
LDIR =../lib

all: autostrata

_DEPS=autostrata.h structs.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=autostrata.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) $(DEBUG) -c -o $@ $< $(CFLAGS)

all: autostrata

autostrata: $(OBJ)
	$(CC) $(DEBUG) -o $@ $^ -L$(LDIR) $(LIBS)

.PHONY: clean debug

debug:
	$(MAKE) $(MAKEFILE) DEBUG="-g"

clean:
	rm -rf $(ODIR)/*.o

$(shell mkdir -p $(ODIR))
