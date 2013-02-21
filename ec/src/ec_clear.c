#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#include "ec.h"

void
ec_clear(ec_point *p)
{
	mpl_clearv(&(p->x), &(p->y), NULL);
}

void
ec_clearv(ec_point *p, ...)
{
	va_list ap;

	va_start(ap, p);

	while (p != NULL) {
		mpl_clearv(&(p->x), &(p->y), NULL);
		p = va_arg(ap, ec_point *);
	}

	va_end(ap);
}

