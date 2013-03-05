#include <stdio.h>
#include <stdint.h>
#include <mpl.h>
#include <ec.h>

#include "geds.h"

int
geds_set_point(geds_point *P, const char *x, int nx, const char *y, int ny)
{
	mpl_int tmp;
	int rc;

	rc = GEDS_OK;

	if (mpl_init(&tmp) != MPL_OK)
		goto err;

	if (mpl_set_uchar(&tmp, (unsigned char *)x, nx) != MPL_OK)
		goto err;

	if (ec_set_x(P, &tmp) != EC_OK)
		goto err;

	if (mpl_set_uchar(&tmp, (unsigned char *)y, ny) != MPL_OK)
		goto err;

	if (ec_set_y(P, &tmp) != EC_OK)
		goto err;

	goto end;
err:
	rc = GEDS_NOMEM;
end:
	mpl_clear(&tmp);

	return rc;
}

int
geds_set_length(int len, geds_context *ctx)
{
	switch (len) {
	case GEDS_LEN_512:
		ctx->length = len;
		ctx->H = gost_hash_256;
		break;

	case GEDS_LEN_1024:
		ctx->length = len;
		ctx->H = gost_hash_512;
		break;

	default:
		return GEDS_ERR;
	}

	return GEDS_OK;
}

int
geds_set_curve(const char *a, int na, const char *b, int nb, const char *p, int np, geds_context *ctx)
{
	int rc;

	if (a != NULL) {
		rc = mpl_set_uchar(&(ctx->a), (unsigned char *)a, na);
		if (rc != MPL_OK)
			goto err;
	}

	if (b != NULL) {
		rc = mpl_set_uchar(&(ctx->b), (unsigned char *)b, nb);
		if (rc != MPL_OK)
			goto err;
	}

	if (p != NULL) {
		rc = mpl_set_uchar(&(ctx->p), (unsigned char *)p, np);
		if (rc != MPL_OK)
			goto err;
	}

	rc = GEDS_OK;
	goto end;
err:
	rc = GEDS_ERR;
end:
	return rc;
}

int
geds_set_subgroup(const char *q, int nq, const geds_point *P, geds_context *ctx)
{
	int rc;

	if (q != NULL) {
		rc = mpl_set_uchar(&(ctx->q), (unsigned char *)q, nq);
		if (rc != MPL_OK)
			goto err;
	}

	if (P != NULL) {
		rc = ec_copy(&(ctx->P), P);
		if (rc != EC_OK)
			goto err;
	}

	rc = GEDS_OK;
	goto end;
err:
	rc = GEDS_ERR;
end:
	return rc;
}

void
geds_set_rnd(int (*rnd)(void *buf, size_t len, void *rndctx), void *rndctx, geds_context *ctx)
{
	ctx->rnd = rnd;
	ctx->rndctx = rndctx;
}

void
geds_set_signature(const unsigned char *buf, geds_context *ctx)
{
	int i;

	for (i = 0; i < ctx->length/8; i++)
		ctx->s[i] = buf[i];
}

