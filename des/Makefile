CC = gcc

CFLAGS = -Wall -I include

headers = des.h

sources = block.c key.c cbc.c

objects = block.o key.o cbc.o

lib = libdes.so

binaries = btest ktest cbctest

vpath %.c src
vpath %.h include

$(lib): %.so: $(objects)
	$(CC) -shared -Wl,-soname,$@ -o $@ $^

$(binaries): %: %.c $(objects)
	$(CC) $(CFLAGS) -o $@ $^

$(objects): %.o: %.c $(headers)
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

.PHONY: clean
clean:
	rm -rf $(binaries) $(objects) $(lib)

