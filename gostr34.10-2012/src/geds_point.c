#include <stdlib.h>

#include "geds.h"

geds_point *
geds_point_new()
{
	ec_point *P;

	P = malloc(sizeof(*P));
	if (P != NULL)
		if (ec_init(P) != EC_OK)
			free(P);

	return P;
}

void
geds_point_free(geds_point **P)
{
	ec_clear(*P);
	free(*P);
	P = NULL;
}

