#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#include "ec.h"

int
ec_init(ec_point *p)
{
	p->type = EC_REGULAR;

	return mpl_initv(&(p->x), &(p->y), NULL);
}

int
ec_initv(ec_point *p, ...)
{
	va_list ap, ap_copy;
	ec_point *pp;
	int cnt;

	pp = p;
	va_start(ap, p);
	va_copy(ap_copy, ap);

	while (pp != NULL) {
		pp->type = EC_REGULAR;
		if (mpl_initv(&(pp->x), &(pp->y), NULL) != MPL_OK)
			goto error;

		cnt++;
		pp = va_arg(ap, ec_point *);
	}

	va_end(ap_copy);
	va_end(ap);

	return EC_OK;
error:
	pp = p;
	while ((pp != NULL) && (cnt > 0)) {
		cnt--;
		mpl_clearv(&(pp->x), &(pp->y), NULL);
		pp = va_arg(ap_copy, ec_point *);
	}

	va_end(ap_copy);
	va_end(ap);

	return EC_NOMEM;
}

