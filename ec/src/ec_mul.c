#include <stdio.h>
#include <stdint.h>

#include "ec.h"

#define	ON_ERROR_GOTO(label, err) {\
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

int
ec_mul(ec_point *x2, const ec_point *x1, const mpl_int *n, const mpl_int *a, const mpl_int *p)
{
	ec_point tmp;
	long int i, nbits;
	int rc;

	rc = ec_init(&tmp);
	ON_ERROR_GOTO(end, rc)

	rc = ec_copy(&tmp, x1);
	ON_ERROR_GOTO(error, rc)

	ec_set_inf(x2);
	nbits = mpl_nr_bits(n);

	for (i = 0; i < nbits-1; i++) {
		if (mpl_hasbit(n, i) == 1) {
			rc = ec_add(x2, x2, &tmp, a, p);
			ON_ERROR_GOTO(error, rc)
		}

		rc = ec_add(&tmp, &tmp, &tmp, a, p);
		ON_ERROR_GOTO(error, rc)
	}

	if (mpl_check_bit(n, i) == 1) {
		rc = ec_add(x2, x2, &tmp, a, p);
		ON_ERROR_GOTO(error, rc)
	}

	rc = EC_OK;
error:
	ec_clear(&tmp);
end:
	return rc;
}

