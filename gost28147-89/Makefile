CC = gcc

CFLAGS = -Wall -I include

headers = gost89.h

sources = block.c gamming.c

objects = block.o gamming.o

lib = libgost89.so

binaries = btest gtest gcbtest

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

