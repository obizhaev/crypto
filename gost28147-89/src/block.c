#include <stdio.h>

#define	LEFT(a)		RIGHT(a >> 4)
#define	RIGHT(a)	(0x0f & (a))

#define	GET_BYTE(n)							\
	e[n] = (sbox[16*n+LEFT(e[n])] << 4) | sbox[16*n+RIGHT(e[n])];

#define	GOST89_ROUND(msg, key)						\
									\
	s = (*(msg) + *(key));						\
	GET_BYTE(0);							\
	GET_BYTE(1);							\
	GET_BYTE(2);							\
	GET_BYTE(3);							\
									\
	s  = (s << 11) | ((s >> (32-11)) & 0x001fffff);	\
	s ^= *(msg+1);							\
									\
	*(msg+1) = *(msg);						\
	*(msg) = s;							\


char sbox[128] = {
	0x4, 0xa, 0x9, 0x2, 0xd, 0x8, 0x0, 0xe,
	0x6, 0xb, 0x1, 0xc, 0x7, 0xf, 0x5, 0x3,

	0xe, 0xb, 0x4, 0xc, 0x6, 0xd, 0xf, 0xa,
	0x2, 0x3, 0x8, 0x1, 0x0, 0x7, 0x5, 0x9,

	0x5, 0x8, 0x1, 0xd, 0xa, 0x3, 0x4, 0x2,
	0xe, 0xf, 0xc, 0x7, 0x6, 0x0, 0x9, 0xb,

	0x7, 0xd, 0xa, 0x1, 0x0, 0x8, 0x9, 0xf,
	0xe, 0x4, 0x6, 0xc, 0xb, 0x2, 0x5, 0x3,

	0x6, 0xc, 0x7, 0x1, 0x5, 0xf, 0xd, 0x8,
	0x4, 0xa, 0x9, 0xe, 0x0, 0x3, 0xb, 0x2,

	0x4, 0xb, 0xa, 0x0, 0x7, 0x2, 0x1, 0xd,
	0x3, 0x6, 0x8, 0x5, 0x9, 0xc, 0xf, 0xe,

	0xd, 0xb, 0x4, 0x1, 0x3, 0xf, 0x5, 0x9,
	0x0, 0xa, 0xe, 0x7, 0x6, 0x8, 0x2, 0xc,

	0x1, 0xf, 0xd, 0x0, 0x5, 0x7, 0xa, 0x4,
	0x9, 0x2, 0x3, 0xe, 0x6, 0xb, 0x8, 0xc
};

void
gost89_block_encrypt(char *m, const char *k)
{
	unsigned int s;
	unsigned int *_m, *_k;
	char *e;

	e = (char *)&s;
	_m = (unsigned int *)m;
	_k = (unsigned int *)k;

	GOST89_ROUND(_m, _k)
	GOST89_ROUND(_m, _k+1)
	GOST89_ROUND(_m, _k+2)
	GOST89_ROUND(_m, _k+3)
	GOST89_ROUND(_m, _k+4)
	GOST89_ROUND(_m, _k+5)
	GOST89_ROUND(_m, _k+6)
	GOST89_ROUND(_m, _k+7)
	GOST89_ROUND(_m, _k)
	GOST89_ROUND(_m, _k+1)
	GOST89_ROUND(_m, _k+2)
	GOST89_ROUND(_m, _k+3)
	GOST89_ROUND(_m, _k+4)
	GOST89_ROUND(_m, _k+5)
	GOST89_ROUND(_m, _k+6)
	GOST89_ROUND(_m, _k+7)
	GOST89_ROUND(_m, _k)
	GOST89_ROUND(_m, _k+1)
	GOST89_ROUND(_m, _k+2)
	GOST89_ROUND(_m, _k+3)
	GOST89_ROUND(_m, _k+4)
	GOST89_ROUND(_m, _k+5)
	GOST89_ROUND(_m, _k+6)
	GOST89_ROUND(_m, _k+7)
	GOST89_ROUND(_m, _k+7)
	GOST89_ROUND(_m, _k+6)
	GOST89_ROUND(_m, _k+5)
	GOST89_ROUND(_m, _k+4)
	GOST89_ROUND(_m, _k+3)
	GOST89_ROUND(_m, _k+2)
	GOST89_ROUND(_m, _k+1)
	GOST89_ROUND(_m, _k)

	s = *_m;
	*_m = *(_m+1);
	*(_m+1) = s;
}

void
gost89_block_decrypt(char *m, const char *k)
{
	unsigned int s;
	unsigned int *_m, *_k;
	char *e;

	e = (char *)&s;
	_m = (unsigned int *)m;
	_k = (unsigned int *)k;

	GOST89_ROUND(_m, _k)
	GOST89_ROUND(_m, _k+1)
	GOST89_ROUND(_m, _k+2)
	GOST89_ROUND(_m, _k+3)
	GOST89_ROUND(_m, _k+4)
	GOST89_ROUND(_m, _k+5)
	GOST89_ROUND(_m, _k+6)
	GOST89_ROUND(_m, _k+7)
	GOST89_ROUND(_m, _k+7)
	GOST89_ROUND(_m, _k+6)
	GOST89_ROUND(_m, _k+5)
	GOST89_ROUND(_m, _k+4)
	GOST89_ROUND(_m, _k+3)
	GOST89_ROUND(_m, _k+2)
	GOST89_ROUND(_m, _k+1)
	GOST89_ROUND(_m, _k)
	GOST89_ROUND(_m, _k+7)
	GOST89_ROUND(_m, _k+6)
	GOST89_ROUND(_m, _k+5)
	GOST89_ROUND(_m, _k+4)
	GOST89_ROUND(_m, _k+3)
	GOST89_ROUND(_m, _k+2)
	GOST89_ROUND(_m, _k+1)
	GOST89_ROUND(_m, _k)
	GOST89_ROUND(_m, _k+7)
	GOST89_ROUND(_m, _k+6)
	GOST89_ROUND(_m, _k+5)
	GOST89_ROUND(_m, _k+4)
	GOST89_ROUND(_m, _k+3)
	GOST89_ROUND(_m, _k+2)
	GOST89_ROUND(_m, _k+1)
	GOST89_ROUND(_m, _k)

	s = *_m;
	*_m = *(_m+1);
	*(_m+1) = s;
}

