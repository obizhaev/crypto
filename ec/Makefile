CC = gcc

CFLAGS = -I include -Wall -lmpl

bindir = bin

libdir = lib

objdir = obj

headers = ec.h

sources = ec_add.c ec_clear.c ec_init.c ec_mul.c ec_set.c ec_util.c

objects = ec_add.o ec_clear.o ec_init.o ec_mul.o ec_set.o ec_util.o

libfile = libec.so

vpath %.h include
vpath %.c src
vpath %.o obj

$(libfile): %.so:
	make objects
	$(CC) -shared -Wl,-soname,$@ -o $(libdir)/$@ $^

%: %.c
	make objects
	$(CC) $(CFLAGS) -o $(bindir)/$@ $^

objects: $(objects)

$(objects): %.o: %.c $(headers)
	$(CC) $(CFLAGS) -fPIC -c -o $(objdir)/$@ $<

.PHONY: install uninstall clean
install:
	install $(libfile) /usr/lib/
	install include/ec.h /usr/include/

uninstall:
	rm -f /usr/lib/$(libfile)
	rm -f /usr/include/ec.h

clean:
	rm -f *.o $(bindir)/* $(libdir)/* $(objdir)/*

