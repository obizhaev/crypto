#include <stdio.h>
#include <stdint.h>
#include <mpl.h>

#include "ec.h"

#define	XX	2
#define	XY	2

#define	N	6

int
main()
{
	ec_point x;
	mpl_int p, a, n;
	char buf[80];

	if (ec_init(&x) != MPL_OK) {
		printf("can't init point");
		return 1;
	}

	if (mpl_initv(&p, &a, &n, NULL) != MPL_OK) {
		printf("can't init ints\n");
		goto err_init;
	}

	mpl_set_sint(&(x.x), XX);
	mpl_set_sint(&(x.y), XY);
	mpl_set_sint(&p, 5);
	mpl_set_sint(&a, 4);
	mpl_set_sint(&n, N);

	if (ec_mul(&x, &x, &n, &a, &p) != MPL_OK) {
		printf("multiplication error\n");
		goto error;
	}

	printf("%i*(%i,%i) = ", N, XX, XY);
	if (mpl_to_str(&(x.x), buf, 10, 80) != MPL_OK) {
		printf("can't convert mpl_int\n");
		goto error;
	}

	printf("(%s,", buf);

	if (mpl_to_str(&(x.y), buf, 10, 80) != MPL_OK) {
		printf("can't convert mpl_int\n");
		goto error;
	}

	printf("%s), ", buf);

	if (x.type == EC_REGULAR)
		printf("regular\n");
	else
		printf("at infinity\n");
error:
	mpl_clearv(&p, &a, &n, NULL);
err_init:
	ec_clear(&x);

	return 0;
}

