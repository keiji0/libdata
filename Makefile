OBJS = memory.o array.o string.o exception.o error.o test.o htable.o io.o mpool.o pair.o map.o
INSTALL = /usr/bin/install
COMPILE = gcc -Wall -O2 -c
SUBDIR = $(shell ls -F | grep "\/$'")

.PHONY: all clean test install

all: libdata.a

s: s.c libdata.a
	gcc -Wall -O2 -o s s.c -L./ -ldata

install: libdata.a libdata.h
	$(INSTALL) -m 444 libdata.a $(HOME)/lib;
	ranlib $(HOME)/lib/libdata.a
	$(INSTALL) -m 444 libdata.h $(HOME)/include;

test: libdata.a
	@for i in $(OBJS:.o=.test); do\
		make $$i;\
	done
	@for i in $(OBJS:.o=.test); do\
		./test $$i;\
	done

clean:
	rm -f *.o *.test *.a

%_extern.h: %.c
	bin/filter_extern $< > $@

%.h: %.c
	make $(addsuffix _extern.h, $(basename $<))
	bin/print_header $< $(addsuffix _extern.h, $(basename $<)) $(addsuffix _type.h, $(basename $<)) > $@

libdata.a: $(OBJS)
	ar rcs $@ $(OBJS)

.c.o: libdata.h
	$(COMPILE) $<

%.test: %.c libdata.h
	gcc -Wall -O2 -D_TEST -o $@ $< -L./ -ldata

symbol: symbol.c
	gcc -o symbol symbol.c
