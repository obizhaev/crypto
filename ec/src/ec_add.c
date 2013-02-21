#include <stdio.h>
#include <stdint.h>

#include "ec.h"

#define	check(a, label)							\
	if (a != MPL_OK) {						\
		goto label;						\
	}

#define	PREPARE_FOR_BARRETT(x, n, p, buf)				\
	if (n <= mpl_nr_bits(x)) {					\
		rc = mpl_div(buf, x, x, p);				\
		check(rc, error)					\
	}

#define	_MODULAR_ADD(c, a, b, p, mu, label)				\
	rc = mpl_add(c, a, b);						\
	check(rc, label)						\
									\
	rc = mpl_reduce_barrett(c, c, p, mu);				\
	check(rc, label)

#define	_MODULAR_SUB(c, a, b, p, mu, label)				\
	rc = mpl_sub(c, a, b);						\
	check(rc, label)						\
									\
	while (mpl_isneg(c)) {						\
		rc = mpl_add(c, c, p);					\
		check(rc, label)					\
	}								\
									\
	rc = mpl_reduce_barrett(c, c, p, mu);				\
	check(rc, label)

#define	_MODULAR_MUL(c, a, b, p, mu, label)				\
	rc = mpl_mul(c, a, b);						\
	check(rc, label)						\
									\
	rc = mpl_reduce_barrett(c, c, p, mu);				\
	check(rc, label)

#define	_MODULAR_SQR(c, a, p, mu, label)				\
	rc = mpl_sqr(c, a);						\
	check(rc, label)						\
									\
	rc = mpl_reduce_barrett(c, c, p, mu);				\
	check(rc, label)

#define	_MODULAR_INVERSE(c, a, p, label)				\
	rc = mpl_mod_inv(c, a, p);					\
	check(rc, label)

#define	ADD_MOD_P(c, a, b)	_MODULAR_ADD(c, a, b, p, &mu_p, error)
#define	SUB_MOD_P(c, a, b)	_MODULAR_SUB(c, a, b, p, &mu_p, error)
#define	MUL_MOD_P(c, a, b)	_MODULAR_MUL(c, a, b, p, &mu_p, error)
#define	SQR_MOD_P(c, a)		_MODULAR_SQR(c, a, p, &mu_p, error)
#define	INV_MOD_P(c, a)		_MODULAR_INVERSE(c, a, p, error)

int
ec_add(ec_point *q3, const ec_point *q1, const ec_point *q2, const mpl_int *a, const mpl_int *p)
{
	ec_point pt1, pt2;
	mpl_int s, mu_p, tmp, a_copy;
	mpl_int *x1, *y1;
	mpl_int *x2, *y2;
	mpl_int *x3, *y3;
	int edge;
	int rc;

	if (mpl_isneg(p)) {
		rc = EC_ERR;
		goto end;
	}

	if (ec_isinf(q1)) {
		rc = ec_copy(q3, q2);
		goto end;
	}

	if (ec_isinf(q2)) {
		rc = ec_copy(q3, q1);
		goto end;
	}

	rc = mpl_initv(&s, &mu_p, &tmp, &a_copy, NULL);
	check(rc, end)

	rc = ec_initv(&pt1, &pt2, NULL);
	check(rc, error_init)

	rc = ec_copy(&pt1, q1);
	check(rc, error)

	rc = ec_copy(&pt2, q2);
	check(rc, error)

	rc = mpl_copy(&a_copy, a);
	check(rc, error)

	x1 = &(pt1.x);
	y1 = &(pt1.y);
	x2 = &(pt2.x);
	y2 = &(pt2.y);
	x3 = &(q3->x);
	y3 = &(q3->y);

	edge = 2*mpl_nr_bits(p);

	PREPARE_FOR_BARRETT(x1, edge, p, &tmp)
	PREPARE_FOR_BARRETT(y1, edge, p, &tmp)
	PREPARE_FOR_BARRETT(x2, edge, p, &tmp)
	PREPARE_FOR_BARRETT(y1, edge, p, &tmp)
	PREPARE_FOR_BARRETT(&a_copy,  edge, p, &tmp)

	rc = mpl_reduce_barrett_setup(&mu_p, p);
	check(rc, error)

	if (mpl_cmp(x1, x2) != MPL_CMP_EQ) {

		/* calculate slope */
		SUB_MOD_P(&tmp, x2, x1)
		INV_MOD_P(&tmp, &tmp)
		SUB_MOD_P(&s, y2, y1)
		MUL_MOD_P(&s, &s, &tmp)

		/* calculate x3 */
		SQR_MOD_P(x3, &s)
		SUB_MOD_P(x3, x3, x1)
		SUB_MOD_P(x3, x3, x2)

		/* calculate y3 */
		SUB_MOD_P(y3, x1, x3)
		MUL_MOD_P(y3, &s, y3)
		SUB_MOD_P(y3, y3, y1)

	} else if (mpl_cmp(y1, y2) == MPL_CMP_EQ) {

		if (mpl_iszero(y1) && mpl_iszero(y2)) {
			rc = EC_ERR;
			goto error;
		}

		/* calculate slope */
		mpl_set_sint(&tmp, 3);
		PREPARE_FOR_BARRETT(&tmp, edge, p, &s)

		SQR_MOD_P(&s, x1)
		MUL_MOD_P(&s, &s, &tmp)
		ADD_MOD_P(&s, &s, &a_copy)
		ADD_MOD_P(&tmp, y1, y1)
		INV_MOD_P(&tmp, &tmp)
		MUL_MOD_P(&s, &s, &tmp)


		/* calculate x3 */
		SQR_MOD_P(x3, &s)
		ADD_MOD_P(&tmp, x1, x1)
		SUB_MOD_P(x3, x3, &tmp)

		/* calculate y3 */
		SUB_MOD_P(y3, x1, x3)
		MUL_MOD_P(y3, &s, y3)
		SUB_MOD_P(y3, y3, y1)
	} else {
		ec_set_type(q3, EC_AT_INF);
	}

	rc = EC_OK;
error:
	ec_clearv(&pt1, &pt2, NULL);
error_init:
	mpl_clearv(&s, &mu_p, &tmp, &a_copy, NULL);
end:
	return rc;
}

