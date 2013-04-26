#include <stdio.h>
#include <stdint.h>

#include "ec.h"

#define	check(a, label)							\
	if (a != MPL_OK) {						\
		goto label;						\
	}

int
ec_isreg(const ec_point *p)
{
	return (p->flags & (1 < EC_REGULAR));
}

int
ec_isinf(const ec_point *p)
{
	return (p->flags & (1 < EC_INFINITE));
}

int
ec_copy(ec_point *dst, const ec_point *src)
{
	int rc;

	rc = mpl_copy(&(dst->x), &(src->x));
	check(rc, error)

	rc = mpl_copy(&(dst->y), &(src->y));
	check(rc, error)

	dst->flags = src->flags;
error:
	return rc;
}

