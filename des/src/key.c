#include "des.h"

#define	_DES_CHECKSUM(b) \
(0x1 & ~((b>>7) ^ (b>>6) ^ (b>>5) ^ (b>>4) ^ (b>>3) ^ (b>>2) ^ (b>>1)))

#define	_DES_GET_64_BITS(n) \
	pool   = (prev << (8-n)) | (src[n] >> n); \
	prev   = src[n]; \
	dst[n] = (pool & ~((unsigned char)0x1)) | _DES_CHECKSUM(pool);

#define	_DES_GET_56_BITS(n) \
	dst[n] = ((src[n] >> 1) << (n+1)) | (src[n+1] >> (7-n));

static void
_add_parity_bits(const unsigned char *src, unsigned char *dst)
{
	char pool, prev;

	prev = 0;
	_DES_GET_64_BITS(0)
	_DES_GET_64_BITS(1)
	_DES_GET_64_BITS(2)
	_DES_GET_64_BITS(3)
	_DES_GET_64_BITS(4)
	_DES_GET_64_BITS(5)
	_DES_GET_64_BITS(6)
	_DES_GET_64_BITS(7)
}

static void
_cut_parity_bits(const unsigned char *src, unsigned char *dst)
{
	_DES_GET_56_BITS(0)
	_DES_GET_56_BITS(1)
	_DES_GET_56_BITS(2)
	_DES_GET_56_BITS(3)
	_DES_GET_56_BITS(4)
	_DES_GET_56_BITS(5)
	_DES_GET_56_BITS(6)
}

int
des_key_transform(const unsigned char *src, unsigned char *dst, int n)
{
	switch (n) {
	case 64:
		_add_parity_bits(src, dst);
		break;
	case 56:
		_cut_parity_bits(src, dst);
		break;
	default:
		return 1;
	}

	return 0;
}

