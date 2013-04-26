#include <stdio.h>
#include <stdint.h>

#include "ec.h"

int
ec_set_x(ec_point *p, const mpl_int *a)
{
	return mpl_copy(&(p->x), a);
}

int
ec_set_y(ec_point *p, const mpl_int *a)
{
	return mpl_copy(&(p->y), a);
}

void
ec_set_reg(ec_point *p)
{
	p->flags = 1 << EC_REGULAR;
}

void
ec_set_inf(ec_point *p)
{
	p->flags = 1 << EC_INFINITE;
}

int
ec_get_x(mpl_int *a, const ec_point *p)
{
	return mpl_copy(a, &(p->x));
}

int
ec_get_y(mpl_int *a, const ec_point *p)
{
	return mpl_copy(a, &(p->y));
}

