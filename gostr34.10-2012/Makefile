CC = gcc

CFLAGS = -I include -Wall -lmpl -lec -lghash

headers = geds.h

sources = geds_context.c geds_set.c geds_get.c geds_point.c geds_check.c \
	  geds_generate.c

objects = geds_context.o geds_set.o geds_get.o geds_point.o geds_check.o \
	  geds_generate.o

libfile = libgeds.so

vpath %.c src test
vpath %.h include

$(libfile): %.so: $(objects)
	$(CC) -shared -Wl,-soname,$@ -o $@ $^

.PHONY: stat timing 
stat: stat.c
	make timing
	$(CC) $(CFLAGS) -o $@ $<
	/bin/bash ./stat.sh

timing: generate_test.c $(sources) $(headers)
	$(CC) $(CFLAGS) -DTIMING -o $@ $^

%: %.c $(objects) $(headers)
	$(CC) $(CFLAGS) -o $@ $^

$(objects): %.o: %.c $(headers)
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

.PHONY: install uninstall clean
install:
	install $(libfile) /usr/lib/
	install ./include/geds.h /usr/include/
	cd ./man/man3; make; make install

uninstall:
	rm -f /usr/lib/$(libfile)
	rm -f /usr/include/geds.h
	cd ./man/man3; make uninstall

clean:
	rm -f $(libfile) $(objects) timing measures stat

