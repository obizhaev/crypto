#ifndef	EC_H_
#define	EC_H_

#include <mpl.h>

#define	EC_REGULAR	0
#define	EC_AT_INF	1

enum {
	EC_OK = MPL_OK,
	EC_ERR = MPL_ERR,
	EC_NOMEM = MPL_NOMEM
};

typedef struct ec_point {
	mpl_int x;
	mpl_int y;
	int type;
} ec_point;

int ec_init(ec_point *p);
int ec_initv(ec_point *p, ...);
void ec_clear(ec_point *p);
void ec_clearv(ec_point *p, ...);

int ec_set_x(ec_point *p, const mpl_int *a);
int ec_set_y(ec_point *p, const mpl_int *a);
void ec_set_type(ec_point *p, int type);

int ec_get_x(mpl_int *a, const ec_point *p);
int ec_get_y(mpl_int *a, const ec_point *p);

int ec_isreg(const ec_point *p);
int ec_isinf(const ec_point *p);

int ec_copy(ec_point *dst, const ec_point *src);

int ec_add(ec_point *x3, const ec_point *x1, const ec_point *x2, const mpl_int *a, const mpl_int *p);
int ec_mul(ec_point *x2, const ec_point *x1, const mpl_int *n, const mpl_int *a, const mpl_int *p);

#endif

