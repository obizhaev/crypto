#include <stdio.h>
#include <stdint.h>
#include <mpl.h>

#include "ec.h"

#define	XX	1
#define	XY	3
#define	YX	1
#define	YY	3

int
main()
{
	mpl_int a;
	int i;

	if (mpl_init(&a) != MPL_OK) {
		printf("can't init\n");
		goto end;
	}

	a.top = 1;
	a.dig[0] = 0x09090909;
	a.dig[1] = 0x09090909;

	for (i = 0; i < 8; i++)
		printf("%i: %i\n", i, mpl_check_bit(&a, i));

	for (i = 48; i < 64; i++)
		printf("%i: %i\n", i, mpl_check_bit(&a, i));

	mpl_clear(&a);
end:
	return 0;
}

