/*
	Gost (R 34.10-2012) Electronic Digital Signature
 
	Copyleft (C) 2013  Kostya Nikulov

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	<knikulov@yandex.com>
*/

#ifndef	GEDS_H_
#define	GEDS_H_

#include <stdio.h>
#include <stdint.h>
#include <limits.h>

#include <mpl.h>
#include <ec.h>
#include <ghash.h>

typedef enum {
	GEDS_LEN_512 = 512 / CHAR_BIT,
	GEDS_LEN_1024 = 1024 / CHAR_BIT,
	GEDS_LEN_UNKNOWN
} geds_len_t;

enum {
	GEDS_OK = 0,
	GEDS_ERR = -1,
	GEDS_NOMEM,
	GEDS_WRONG
};

typedef ec_point geds_point;

typedef struct geds_context {
	/* signature length: 512 or 1024 */
	geds_len_t length;

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
	int (*H)(const unsigned char *msg, int n, unsigned char *hash);

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

int geds_set_point(geds_point *P, const unsigned char *x, int nx, const unsigned char *y, int ny);
int geds_set_length(geds_context *ctx, int len);
int geds_set_curve(geds_context *ctx, const unsigned char *a, int na, const unsigned char *b, int nb, const unsigned char *p, int np);
int geds_set_subgroup(geds_context *ctx, const unsigned char *q, int nq, const geds_point *P);
void geds_set_rnd(geds_context *ctx, int (*rnd)(void *buf, size_t len, void *rndctx), void *rndctx);
void geds_set_signature(geds_context *ctx, const unsigned char *buf);

void geds_get_signature(const geds_context *ctx, unsigned char *buf);

int geds_generate(geds_context *ctx, char *msg, int n, unsigned char *d, int nd);
int geds_verify(const geds_context *ctx, char *msg, int n, const geds_point *Q);

#endif

