#include "gost89.h"

static void
gost89_gamming(char *m, int n, const char *k, char *s)
{
	int i, q, r;
	unsigned int *msg, *key, *syn;

	msg = (unsigned int *)m;
	key = (unsigned int *)k;
	syn = (unsigned int *)s;
	q = n/8;
	r = n%8;

	gost89_block_encrypt((char *)syn, (char *)key);

	for (i = 0; i < q; i++) {
		syn[0] =  syn[0] + 0x1010101;
		syn[1] = (syn[1] + 0x1010104) % 0xffffffffU;

		gost89_block_encrypt((char *)syn, (char *)key);

		msg[0] ^= syn[0];
		msg[1] ^= syn[1];

		msg += 2;
	}

	syn[0] =  syn[0] + 0x1010101;
	syn[1] = (syn[1] + 0x1010104) % 0xffffffffU;

	gost89_block_encrypt((char *)syn, (char *)key);

	m = (char *)msg;
	s = (char *)syn;

	for (i = 0; i < r; i++)
		m[i] ^= s[i];
}

void
gost89_gamming_encrypt(char *m, int n, const char *k, char *s)
{
	gost89_gamming(m, n, k, s);
}

void
gost89_gamming_decrypt(char *m, int n, const char *k, char *s)
{
	gost89_gamming(m, n, k, s);
}

void
gost89_gamming_cb_encrypt(char *m, int n, const char *k, char *s)
{
	int i, q, r;
	unsigned int *msg, *key, *syn;

	msg = (unsigned int *)m;
	key = (unsigned int *)k;
	syn = (unsigned int *)s;
	q = n/8;
	r = n%8;

	for (i = 0; i < q; i++) {
		gost89_block_encrypt((char *)syn, (char *)key);

		msg[0] ^= syn[0];
		msg[1] ^= syn[1];

		syn[0] = msg[0];
		syn[1] = msg[1];

		msg += 2;
	}

	gost89_block_encrypt((char *)syn, (char *)key);

	m = (char *)msg;
	s = (char *)syn;

	for (i = 0; i < r; i++)
		m[i] ^= s[i];
}

void
gost89_gamming_cb_decrypt(char *m, int n, const char *k, char *s)
{
	int tmp[2];
	int i, q, r;
	unsigned int *msg, *key, *syn;

	msg = (unsigned int *)m;
	key = (unsigned int *)k;
	syn = (unsigned int *)s;
	q = n/8;
	r = n%8;

	for (i = 0; i < q; i++) {
		gost89_block_encrypt((char *)syn, (char *)key);

		tmp[0] = msg[0];
		tmp[1] = msg[1];

		msg[0] ^= syn[0];
		msg[1] ^= syn[1];

		syn[0] = tmp[0];
		syn[1] = tmp[1];

		msg += 2;
	}

	gost89_block_encrypt((char *)syn, (char *)key);

	m = (char *)msg;
	s = (char *)syn;

	for (i = 0; i < r; i++)
		m[i] ^= s[i];
}

