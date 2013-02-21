#include <stdio.h>
#include <stdint.h>
#include <mpl.h>

#include "ec.h"

#define	XX	2
#define	XY	3
#define	YX	2
#define	YY	3

int
main()
{
	ec_point x, y, z;
	mpl_int p, a;
	char buf[80];

	if (ec_initv(&x, &y, &z, NULL) != MPL_OK) {
		printf("can't init points");
		return 1;
	}

	if (mpl_initv(&p, &a, NULL) != MPL_OK) {
		printf("can't init ints\n");
		goto err_init;
	}

	mpl_set_sint(&(x.x), XX);
	mpl_set_sint(&(x.y), XY);
	mpl_set_sint(&(y.x), YX);
	mpl_set_sint(&(y.y), YY);
	mpl_set_sint(&p, 5);
	mpl_set_sint(&a, 4);

	if (ec_add(&z, &x, &y, &a, &p) != MPL_OK) {
		printf("addition error\n");
		goto error;
	}

	printf("(%i,%i) + (%i,%i) = ", XX, XY, YX, YY);
	if (mpl_to_str(&(z.x), buf, 10, 80) != MPL_OK) {
		printf("can't convert mpl_int\n");
		goto error;
	}

	printf("(%s,", buf);

	if (mpl_to_str(&(z.y), buf, 10, 80) != MPL_OK) {
		printf("can't convert mpl_int\n");
		goto error;
	}

	printf("%s)\n", buf);

error:
	mpl_clearv(&p, &a, NULL);
err_init:
	ec_clearv(&x, &y, &z, NULL);

	return 0;
}

