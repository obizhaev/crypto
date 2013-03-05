#include <stdio.h>
#include <stdint.h>
#include <mpl.h>
#include <ec.h>

#include "geds.h"

void
geds_get_signature(unsigned char *buf, const geds_context *ctx)
{
	int i;

	for (i = 0; i < ctx->length/8; i++)
		buf[i] = ctx->s[i];
}

