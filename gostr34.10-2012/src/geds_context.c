#include <stdlib.h>
#include <string.h>

#include "geds.h"

geds_context *
geds_context_new()
{
	geds_context *ctx;

	ctx = malloc(sizeof(*ctx));
	if (ctx == NULL)
		return NULL;

	if (ec_init(&(ctx->P)) != EC_OK)
		goto err_ec;

	if (mpl_initv(&ctx->a, &ctx->b, &ctx->p, &ctx->q, NULL) != MPL_OK)
		goto err_mpl;

	ctx->length = GEDS_LEN_UNKNOWN;;
	ctx->H = NULL;
	ctx->rnd = NULL;
	ctx->rndctx = NULL;

	memset(ctx->s, 0, 128);

	return ctx;

err_mpl:
	ec_clear(&(ctx->P));
err_ec:
	free(ctx);
	ctx = NULL;

	return ctx;
}

void
geds_context_free(geds_context **ctx)
{
	mpl_clearv(&(*ctx)->a, &(*ctx)->b, &(*ctx)->p, &(*ctx)->q, NULL);
	ec_clear(&((*ctx)->P));
	free(*ctx);
	*ctx = NULL;
}

