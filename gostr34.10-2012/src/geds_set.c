#include <stdio.h>
#include <stdint.h>
#include <mpl.h>
#include <ec.h>

#include "geds.h"

#define	NELEMS(a) (sizeof(a)/sizeof(a[0]))

int
geds_set_point(geds_point *P, const unsigned char *x, int nx, const unsigned char *y, int ny)
{
	mpl_int tmp;
	int rc;

	if (mpl_init(&tmp) != MPL_OK) {
		return GEDS_ERR;
	} else if (mpl_set_uchar(&tmp, x, nx) != MPL_OK) {
		rc = GEDS_ERR;
	} else  if (ec_set_x(P, &tmp) != EC_OK) {
		rc = GEDS_ERR;
	} else if (ec_set_y(P, &tmp) != EC_OK) {
		rc = GEDS_ERR;
	} else {
		rc = GEDS_OK;
	}

	mpl_clear(&tmp);
	return rc;
}

int
geds_set_length(geds_context *ctx, int len)
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
geds_set_curve(geds_context *ctx, const unsigned char *a, int na, const unsigned char *b, int nb, const unsigned char *p, int np)
{
	mpl_int *list[] = {&ctx->a, &ctx->b, &ctx->p};
	const unsigned char *blist[] = {a, b, p};
	int nlist[] = {na, nb, np};
	int i;
	int rc;

	rc = GEDS_OK;
	for (i = 0; i < NELEMS(list); i++) {
		if (blist[i] != NULL) {
			rc = mpl_set_uchar(list[i], blist[i], nlist[i]);
			if (rc != MPL_OK) {
				rc = GEDS_ERR;
				break;
			}
		}
	}

	return rc;
}

int
geds_set_subgroup(geds_context *ctx, const unsigned char *q, int nq, const geds_point *P)
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
geds_set_rnd(geds_context *ctx, int (*rnd)(void *buf, size_t len, void *rndctx), void *rndctx)
{
	ctx->rnd = rnd;
	ctx->rndctx = rndctx;
}

void
geds_set_signature(geds_context *ctx, const unsigned char *buf)
{
	int i;

	for (i = 0; i < ctx->length/8; i++)
		ctx->s[i] = buf[i];
}

