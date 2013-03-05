#include <stdio.h>

#include "geds.h"

int
gost_hash_256(const char *msg, int n, char *buf)
{
	return 0;
}

int
gost_hash_512(const char *msg, int n, char *buf)
{
	return 0;
}

int
main()
{
	geds_context *ctx;

	ctx = geds_context_new();
	if (ctx == NULL) {
		printf("can't init context\n");
		return 1;
	}

	geds_context_free(&ctx);

	return 0;
}

