CC = gcc

CFLAGS = -I include -Wall -lmpl -lec

headers = geds.h

sources = geds_context.c geds_set.c geds_get.c geds_point.c geds_check.c \
	  geds_generate.c

objects = geds_context.o geds_set.o geds_get.o geds_point.o geds_check.o \
	  geds_generate.o

vpath %.c src test
vpath %.h include

%: %.c $(objects) $(headers)
	$(CC) $(CFLAGS) -o $@ $^

$(objects): %.o: %.c $(headers)
	$(CC) $(CFLAGS) -fPIC -c -o $@ $<

.PHONY: clean
clean:
	rm -f *.o

