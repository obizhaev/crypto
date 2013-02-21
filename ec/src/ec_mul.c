#include <stdio.h>
#include <stdint.h>

#include "ec.h"

#define	check(a, label)							\
	if (a != MPL_OK) {						\
		goto label;						\
	}

int
ec_mul(ec_point *x2, const ec_point *x1, const mpl_int *n, const mpl_int *a, const mpl_int *p)
{
	ec_point tmp;
	long int i, nbits;
	int rc;

	rc = ec_init(&tmp);
	check(rc, end)

	rc = ec_copy(&tmp, x1);
	check(rc, error)

	ec_set_type(x2, EC_AT_INF);
	nbits = mpl_nr_bits(n);

	for (i = 0; i < nbits-1; i++) {
		if (mpl_check_bit(n, i) == 1) {
			rc = ec_add(x2, x2, &tmp, a, p);
			check(rc, error)
		}

		rc = ec_add(&tmp, &tmp, &tmp, a, p);
		check(rc, error)
	}

	if (mpl_check_bit(n, i) == 1) {
		rc = ec_add(x2, x2, &tmp, a, p);
		check(rc, error)
	}

	rc = EC_OK;
error:
	ec_clear(&tmp);
end:
	return rc;
}

