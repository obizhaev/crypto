#include <stdio.h>
#include <stdint.h>

#include "ec.h"

#define	ON_ERROR_GOTO(label, err) \
	{\
		int _e; \
		if ((_e = (err)) != MPL_OK) { \
			fprintf(stderr, "error at %s: %d (%s): %s\n", \
				__FILE__, \
				__LINE__, \
				#err, \
				mpl_error_str(_e)); \
			goto label; \
		} \
	}

#define	PREPARE_FOR_BARRETT(x, n, p, buf, label)			\
	if (n <= mpl_nr_bits(x)) {					\
		ON_ERROR_GOTO(label, mpl_div(buf, x, x, p))		\
	}

#define	_MODULAR_ADD(c, a, b, p, mu, label)				\
	do { 								\
		ON_ERROR_GOTO(label, mpl_add(c, a, b))			\
		ON_ERROR_GOTO(label, mpl_reduce_barrett(c, c, p, mu))	\
	} while (0);

#define	_MODULAR_SUB(c, a, b, p, mu, label)				\
	do {								\
		ON_ERROR_GOTO(label, mpl_sub(c, a, b))			\
									\
		while (mpl_isneg(c))					\
			ON_ERROR_GOTO(label, mpl_add(c, c, p))		\
									\
		ON_ERROR_GOTO(label, mpl_reduce_barrett(c, c, p, mu))	\
	} while (0);

#define	_MODULAR_MUL(c, a, b, p, mu, label)				\
	do {								\
		ON_ERROR_GOTO(label, mpl_mul(c, a, b))			\
		ON_ERROR_GOTO(label, mpl_div(&tmp2, c, c, p))		\
	} while (0);

#define	_MODULAR_SQR(c, a, p, mu, label)				\
	do {								\
		ON_ERROR_GOTO(label, mpl_sqr(c, a))			\
		ON_ERROR_GOTO(label, mpl_div(&tmp2, c, c, p))		\
	} while (0);

#define	_MODULAR_INVERSE(c, a, p, label)				\
	do {								\
		ON_ERROR_GOTO(label, mpl_mod_inv(c, a, p))		\
	} while (0);

#define	ADD_MOD_P(c, a, b)	_MODULAR_ADD(c, a, b, p, &mu_p, error)
#define	SUB_MOD_P(c, a, b)	_MODULAR_SUB(c, a, b, p, &mu_p, error)
#define	MUL_MOD_P(c, a, b)	_MODULAR_MUL(c, a, b, p, &mu_p, error)
#define	SQR_MOD_P(c, a)		_MODULAR_SQR(c, a, p, &mu_p, error)
#define	INV_MOD_P(c, a)		_MODULAR_INVERSE(c, a, p, error)

int
ec_add(ec_point *q3, const ec_point *q1, const ec_point *q2, const mpl_int *a, const mpl_int *p)
{
	ec_point pt1, pt2;
	mpl_int s, mu_p, tmp, tmp2, a_copy;
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

	rc = mpl_initv(&s, &mu_p, &tmp, &tmp2, &a_copy, NULL);
	ON_ERROR_GOTO(end, rc)

	rc = ec_initv(&pt1, &pt2, NULL);
	ON_ERROR_GOTO(error_init, rc)

	rc = ec_copy(&pt1, q1);
	ON_ERROR_GOTO(error, rc)

	rc = ec_copy(&pt2, q2);
	ON_ERROR_GOTO(error, rc)

	rc = mpl_copy(&a_copy, a);
	ON_ERROR_GOTO(error, rc)

	x1 = &(pt1.x);
	y1 = &(pt1.y);
	x2 = &(pt2.x);
	y2 = &(pt2.y);
	x3 = &(q3->x);
	y3 = &(q3->y);

	edge = 2*mpl_nr_bits(p);

	PREPARE_FOR_BARRETT(x1, edge, p, &tmp, error)
	PREPARE_FOR_BARRETT(y1, edge, p, &tmp, error)
	PREPARE_FOR_BARRETT(x2, edge, p, &tmp, error)
	PREPARE_FOR_BARRETT(y1, edge, p, &tmp, error)
	PREPARE_FOR_BARRETT(&a_copy,  edge, p, &tmp, error)

	rc = mpl_reduce_barrett_setup(&mu_p, p);
	ON_ERROR_GOTO(error, rc)

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
		PREPARE_FOR_BARRETT(&tmp, edge, p, &s, error)

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
		ec_set_inf(q3);
	}

	rc = EC_OK;
error:
	ec_clearv(&pt1, &pt2, NULL);
error_init:
	mpl_clearv(&s, &mu_p, &tmp, &tmp2, &a_copy, NULL);
end:
	return rc;
}

