#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "geds.h"

#define	on_fail(msg)					\
	if ((rc != GEDS_OK) && (rc != MPL_OK)) {	\
		perror(msg);				\
		goto free_Q;				\
	}						\

static unsigned char p[] = {
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x31
};

static unsigned char a[] = {
	0x07
};

static unsigned char b[] = {
	0x5F, 0xBF, 0xF4, 0x98, 0xAA, 0x93, 0x8C, 0xE7,
	0x39, 0xB8, 0xE0, 0x22, 0xFB, 0xAF, 0xEF, 0x40,
	0x56, 0x3F, 0x6E, 0x6A, 0x34, 0x72, 0xFC, 0x2A,
	0x51, 0x4C, 0x0C, 0xE9, 0xDA, 0xE2, 0x3B, 0x7E
};

static unsigned char msg[] = {
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x50, 0xFE, 0x8A, 0x18, 0x92, 0x97, 0x61, 0x54,
	0xC5, 0x9C, 0xFC, 0x19, 0x3A, 0xCC, 0xF5, 0xB3
};

static unsigned char q[] = {
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x50, 0xFE, 0x8A, 0x18, 0x92, 0x97, 0x61, 0x54,
	0xC5, 0x9C, 0xFC, 0x19, 0x3A, 0xCC, 0xF5, 0xB3
};

static unsigned char px[] = {
	0x02
};

static unsigned char py[] = {
	0x08, 0xE2, 0xA8, 0xA0, 0xE6, 0x51, 0x47, 0xD4,
	0xBD, 0x63, 0x16, 0x03, 0x0E, 0x16, 0xD1, 0x9C,
	0x85, 0xC9, 0x7F, 0x0A, 0x9C, 0xA2, 0x67, 0x12,
	0x2B, 0x96, 0xAB, 0xBC, 0xEA, 0x7E, 0x8F, 0xC8
};

static unsigned char d[] = {
	0x7A, 0x92, 0x9A, 0xDE, 0x78, 0x9B, 0xB9, 0xBE,
	0x10, 0xED, 0x35, 0x9D, 0xD3, 0x9A, 0x72, 0xC1,
	0x1B, 0x60, 0x96, 0x1F, 0x49, 0x39, 0x7E, 0xEE,
	0x1D, 0x19, 0xCE, 0x98, 0x91, 0xEC, 0x3B, 0x28
};

static unsigned char qx[] = {
	0x7F, 0x2B, 0x49, 0xE2, 0x70, 0xDB, 0x6D, 0x90,
	0xD8, 0x59, 0x5B, 0xEC, 0x45, 0x8B, 0x50, 0xC5,
	0x85, 0x85, 0xBA, 0x1D, 0x4E, 0x9B, 0x78, 0x8F,
	0x66, 0x89, 0xDB, 0xD8, 0xE5, 0x6F, 0xD8, 0x0B
};

static unsigned char qy[] = {
	0x26, 0xF1, 0xB4, 0x89, 0xD6, 0x70, 0x1D, 0xD1,
	0x85, 0xC8, 0x41, 0x3A, 0x97, 0x7B, 0x3C, 0xBB,
	0xAF, 0x64, 0xD1, 0xC5, 0x93, 0xD2, 0x66, 0x27,
	0xDF, 0xFB, 0x10, 0x1A, 0x87, 0xFF, 0x77, 0xDA
};

/*
int
gost_hash_256(const char *msg, int n, char *buf)
{
	char h[] = {
		0x2D, 0xFB, 0xC1, 0xB3, 0x72, 0xD8, 0x9A, 0x11,
		0x88, 0xC0, 0x9C, 0x52, 0xE0, 0xEE, 0xC6, 0x1F,
		0xCE, 0x52, 0x03, 0x2A, 0xB1, 0x02, 0x2E, 0x8E,
		0x67, 0xEC, 0xE6, 0x67, 0x2B, 0x04, 0x3E, 0xE5
	};

	int i;

	for (i = 0; i < 32; i++)
		buf[i] = (char)h[i];

	return 0;
}

int
gost_hash_512(const char *msg, int n, char *buf)
{
	int i;

	for (i = 0; i < 64; i++)
		buf[i] = (char)0;

	return 0;
}
*/

int
urandom(void *buf, size_t len)
{
	int fd, res;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		return -1;
	res = read(fd, buf, len);
	close(fd);
	return res;
}

int
rnd(void *buf, size_t len, void *ctx)
{
	return urandom(buf, len);
}

int
main()
{
	geds_context *ctx;
	geds_point *P, *Q;
	mpl_int s;
	int rc;
	char buf[512];

	rc = mpl_init(&s);
	if (rc != MPL_OK) {
		perror("no memory for s");
		goto end;
	}

	ctx = geds_context_new();
	if (ctx == NULL) {
		perror("no memory for ctx");
		goto free_s;
	}

	P = geds_point_new();
	if (P == NULL) {
		perror("no memory for P");
		goto free_ctx;
	}

	Q = geds_point_new();
	if (Q == NULL) {
		perror("no memory for Q");
		goto free_P;
	}


	rc = geds_set_length(ctx, GEDS_LEN_512);
	on_fail("can't set length")

	rc = geds_set_curve(ctx, a, sizeof(a), b, sizeof(b), p, sizeof(p));
	on_fail("can't set curve")

	rc = geds_set_point(P, px, sizeof(px), py, sizeof(py));
	on_fail("can't set P")

	rc = geds_set_point(Q, qx, sizeof(qx), qy, sizeof(qy));
	on_fail("can't set Q")

	rc = geds_set_subgroup(ctx, q, sizeof(q), P);
	on_fail("can't set subgroup")

	geds_set_rnd(ctx, rnd, NULL);

	rc = geds_generate(ctx, msg, sizeof(msg), d, sizeof(d));
	on_fail("can't generate")

	rc = mpl_set_uchar(&s, ctx->s, 64);
	on_fail("can't set s")

	rc = mpl_to_str(&s, buf, 16, sizeof(buf));
	on_fail("can't make str")

#ifndef TIMING
	printf("Signature: %s\n", buf);

	rc = geds_verify(ctx, msg, sizeof(msg), Q);
	switch (rc) {
	case GEDS_OK:
		printf("Signature is ok\n");
		break;
	case GEDS_WRONG:
		printf("Signature is wrong\n");
		break;
	default:
		perror("can't check");
		break;
	};
#endif

free_Q:
	geds_point_free(&Q);
free_P:
	geds_point_free(&P);
free_ctx:
	geds_context_free(&ctx);
free_s:
	mpl_clear(&s);
end:
	return 0;
}

