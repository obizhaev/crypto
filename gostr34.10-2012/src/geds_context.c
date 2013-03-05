#include <stdlib.h>

#include "geds.h"

geds_context *
geds_context_new()
{
	geds_context *new;
	mpl_int *a, *b, *p, *q;
	int i;

	new = malloc(sizeof(*new));
	if (new == NULL)
		goto end;

	if (ec_init(&(new->P)) != EC_OK)
		goto err_ec;

	a = &(new->a);
	b = &(new->b);
	p = &(new->p);
	q = &(new->q);

	if (mpl_initv(a, b, p, q, NULL) != MPL_OK)
		goto err_mpl;

	new->length = -2;
	new->H = NULL;
	new->rnd = NULL;
	new->rndctx = NULL;

	for (i = 0; i < 128; i++)
		new->s[i] = (unsigned char)0;

	goto end;
err_mpl:
	ec_clear(&(new->P));
err_ec:
	free(new);
	new = NULL;
end:
	return new;
}

void
geds_context_free(geds_context **ctx)
{
	mpl_int *a, *b, *p, *q;

	a = &((*ctx)->a);
	b = &((*ctx)->b);
	q = &((*ctx)->q);
	p = &((*ctx)->p);

	mpl_clearv(a, b, p, q, NULL);
	ec_clear(&((*ctx)->P));
	free(*ctx);
	*ctx = NULL;
}

