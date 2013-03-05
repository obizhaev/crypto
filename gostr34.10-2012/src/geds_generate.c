#include <stdint.h>
#include <stdio.h>

#include <mpl.h>
#include <ec.h>

#include "geds.h"

int
geds_generate(char *msg, int n, char *d, int nd, geds_context *ctx)
{
	ec_point C;
	ec_point *P;
	mpl_int e, k, key, r, s, tmp, mu;
	const mpl_int *q, *p, *a;
	int rc;
	unsigned char h[64];

	if (ctx->length < 0) {
		rc = GEDS_ERR;
		goto end;
	}

	rc = mpl_initv(&e, &k, &key, &r, &s, &tmp, &mu, NULL);
	if (rc != MPL_OK) {
		rc =  GEDS_ERR;
		goto end;
	}

	rc = ec_init(&C);
	if (rc != EC_OK) {
		rc = GEDS_ERR;
		goto err_mpl;
	}

	rc = ctx->H(msg, n, (char *)h);
	if (rc != 0) {
		rc = GEDS_ERR;
		goto err_point;
	}

	rc = mpl_set_uchar(&e, h, ctx->length/16);
	if (rc != MPL_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	q = &(ctx->q);
	p = &(ctx->p);
	a = &(ctx->a);
	P = &(ctx->P);

	rc = mpl_div(&tmp, &e, &e, q);
	if (rc != MPL_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	if (mpl_iszero(&e))
		mpl_set_one(&e);

	rc = mpl_reduce_barrett_setup(&mu, q);
	if (rc != MPL_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	rc = mpl_set_uchar(&key, (unsigned char *)d, nd);
	if (rc != MPL_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	rc = mpl_div(&tmp, &key, &key, q);
	if (rc != MPL_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	mpl_zero(&s);
	while (mpl_iszero(&s)) {

		mpl_zero(&r);
		while (mpl_iszero(&r)) {
			rc = mpl_random(&k, mpl_nr_bits(q)/8, ctx->rnd, ctx->rndctx);
			if (rc != MPL_OK) {
				rc = GEDS_ERR;
				goto err_point;
			}

			rc = ec_mul(&C, P, &k, a, p);
			if (rc != EC_OK) {
				rc = GEDS_ERR;
				goto err_point;
			}

			rc = ec_get_x(&r, &C);
			if (rc != EC_OK) {
				rc = GEDS_ERR;
				goto err_point;
			}

			rc = mpl_div(&tmp, &r, &r, q);
			if (rc != MPL_OK) {
				rc = GEDS_ERR;
				goto err_point;
			}
		}

		rc = mpl_mul(&s, &r, &key);
		if (rc != MPL_OK) {
			rc = GEDS_ERR;
			goto err_point;
		}

		rc = mpl_div(&tmp, &s, &s, q);
		if (rc != MPL_OK) {
			rc = GEDS_ERR;
			goto err_point;
		}

		rc = mpl_mul(&k, &k, &e);
		if (rc != MPL_OK) {
			rc = GEDS_ERR;
			goto err_point;
		}

		rc = mpl_div(&tmp, &k, &k, q);
		if (rc != MPL_OK) {
			rc = GEDS_ERR;
			goto err_point;
		}

		rc = mpl_add(&s, &s, &k);
		if (rc != MPL_OK) {
			rc = GEDS_ERR;
			goto err_point;
		}

		rc = mpl_reduce_barrett(&s, &s, q, &mu);
		if (rc != MPL_OK) {
			rc = GEDS_ERR;
			goto err_point;
		}
	}

	rc = mpl_to_uchar(&r, ctx->s, ctx->length/16);
	if (rc != MPL_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	rc = mpl_to_uchar(&s, ctx->s + (ctx->length/16), ctx->length/16);
	if (rc != MPL_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	rc = GEDS_OK;
err_point:
	ec_clear(&C);
err_mpl:
	mpl_clearv(&e, &k, &key, &r, &s, &tmp, &mu, NULL);
end:
	return rc;
}
