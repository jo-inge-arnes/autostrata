IDIR=../include
CC=gcc
CFLAGS=-Wall -I$(IDIR)
LIBS=-lm
ODIR=../obj
LDIR=../lib
OUTPUTFILE=libautostrata.a

_DEPS=autostrata.h structs.h structutils.h initialization.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=autostrata.o structutils.o initialization.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) $(DEBUG) -c -o $@ $< $(CFLAGS)

$(OUTPUTFILE): $(OBJ)
	ar ru $(LDIR)/$@ $^
	ranlib $(LDIR)/$@

.PHONY: all
all: $(OUTPUTFILE)

.PHONY: debug
debug:
	$(MAKE) $(MAKEFILE) DEBUG="-g"

.PHONY: prof
prof:
	$(MAKE) $(MAKEFILE) DEBUG="-pg"

.PHONY: fast
fast:
	$(MAKE) $(MAKEFILE) DEBUG="-Ofast"

.PHONY: clean
clean:
	rm -rf $(ODIR)/*.*

$(shell mkdir -p $(ODIR) $(LDIR))