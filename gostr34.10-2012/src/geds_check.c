#include <stdint.h>
#include <stdio.h>

#include <mpl.h>
#include <ec.h>

#include "geds.h"

int
geds_check(char *msg, int n, const geds_point *Q, const geds_context *ctx)
{
	ec_point C, tmp;
	mpl_int r, s, e, v, z1, z2, xc, mu, tmp2;
	const ec_point *P;
	const mpl_int *q, *p, *a;
	int rc;
	unsigned char h[64];

	rc = mpl_initv(&r, &s, &e, &v, &z1, &z2, &xc, &mu, &tmp2, NULL);
	if (rc != MPL_OK) {
		rc = GEDS_NOMEM;
		goto end;
	}

	rc = ec_initv(&C, &tmp, NULL);
	if (rc != EC_OK) {
		rc = GEDS_NOMEM;
		goto err_mpl;
	}

	q = &(ctx->q);

	rc = mpl_set_uchar(&r, ctx->s, ctx->length/16);
	if (rc != MPL_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	if ((mpl_abs_cmp(&r, q) != MPL_CMP_LT) || mpl_iszero(&r)) {
		rc = GEDS_WRONG;
		goto err_point;
	}

	rc = mpl_set_uchar(&s, ctx->s + (ctx->length/16), ctx->length/16);
	if (rc != MPL_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	if ((mpl_abs_cmp(&s, q) != MPL_CMP_LT) || mpl_iszero(&s)) {
		rc = GEDS_WRONG;
		goto err_point;
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

	p = &(ctx->p);
	a = &(ctx->a);
	P = &(ctx->P);

	rc = mpl_div(&mu, &e, &e, q);
	if (rc != MPL_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	if (mpl_iszero(&e))
		mpl_set_one(&e);

	rc = mpl_mod_inv(&v, &e, q);
	if (rc != MPL_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	rc = mpl_reduce_barrett_setup(&mu, q);
	if (rc != MPL_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	rc = mpl_mul(&z1, &s, &v);
	if (rc != MPL_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	rc = mpl_div(&tmp2, &z1, &z1, q);
	if (rc != MPL_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	rc = mpl_mul(&z2, &r, &v);
	if (rc != MPL_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	rc = mpl_div(&tmp2, &z2, &z2, q);
	if (rc != MPL_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	rc = mpl_sub(&z2, q, &z2);
	if (rc != MPL_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	rc = ec_mul(&C, P, &z1, a, p);
	if (rc != EC_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	rc = ec_mul(&tmp, Q, &z2, a, p);
	if (rc != EC_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	rc = ec_add(&C, &C, &tmp, a, p);
	if (rc != EC_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	rc = ec_get_x(&xc, &C);
	if (rc != EC_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	rc = mpl_div(&tmp2, &xc, &xc, q);
	if (rc != MPL_OK) {
		rc = GEDS_ERR;
		goto err_point;
	}

	if (mpl_abs_cmp(&r, &xc) == MPL_CMP_EQ)
		rc = GEDS_OK;
	else
		rc = GEDS_WRONG;
err_point:
	ec_clearv(&C, &tmp, NULL);
err_mpl:
	mpl_clearv(&r, &s, &e, &v, &z1, &z2, &xc, &mu, &tmp2, NULL);
end:
	return rc;
}

