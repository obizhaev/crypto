/*
 * Gost (R 34.10-2012) Electronic Digital Signature
 *
 * Kostya Nikulov, 2013
 * <knikulov@yandex.com>
 */

#ifndef	GEDS_H_
#define	GEDS_H_

#include <stdio.h>
#include <stdint.h>

#include <mpl.h>
#include <ec.h>
#include "ghash.h"

enum {
	GEDS_LEN_512 = 512,
	GEDS_LEN_1024 = 1024
};

enum {
	GEDS_OK = 0,
	GEDS_ERR = -1,
	GEDS_NOMEM,
	GEDS_WRONG
};

typedef ec_point geds_point;

typedef struct geds_context {
	/* signature length: 512 or 1024 */
	int length;

	/* quotients of elliptic curve */
	mpl_int a;
	mpl_int b;

	/* prime number, modulus of curve */
	mpl_int p;

	/* prime number, order of cyclic points subgroup */
	mpl_int q;

	/* curve point of order q */
	geds_point P;

	/* hash function */
	int (*H)(const char *msg, int n, char *hash);

	/* entropy source */
	int (*rnd)(void *buf, size_t len, void *rndctx);

	/* entropy source context */
	void *rndctx;

	/* signature buffer */
	unsigned char s[128];
} geds_context;

geds_context *geds_context_new();
void geds_context_free(geds_context **ctx);

geds_point *geds_point_new();
void geds_point_free(geds_point **P);

int geds_set_point(geds_point *P, const char *x, int nx, const char *y, int ny);
int geds_set_length(int len, geds_context *ctx);
int geds_set_curve(const char *a, int na, const char *b, int nb, const char *p, int np, geds_context *ctx);
int geds_set_subgroup(const char *q, int nq, const geds_point *P, geds_context *ctx);
void geds_set_rnd(int (*rnd)(void *buf, size_t len, void *rndctx), void *rndctx, geds_context *ctx);
void geds_set_signature(const unsigned char *buf, geds_context *ctx);

void geds_get_signature(unsigned char *buf, const geds_context *ctx);

int geds_generate(char *msg, int n, char *d, int nd, geds_context *ctx);
int geds_check(char *msg, int n, const geds_point *Q, const geds_context *ctx);

#endif

