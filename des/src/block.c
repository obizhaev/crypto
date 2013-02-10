#include "des.h"

#define	COPY_4(src, dst)						\
	(dst)[0] = (src)[0];						\
	(dst)[1] = (src)[1];						\
	(dst)[2] = (src)[2];						\
	(dst)[3] = (src)[3];						\

#define	COPY_6(src, dst)						\
	COPY_4(src, dst)						\
	(dst)[4] = (src)[4];						\
	(dst)[5] = (src)[5];

#define	COPY_7(src, dst)						\
	COPY_6(src, dst)						\
	(dst)[6] = (src)[6];

#define	COPY_8(src, dst)						\
	COPY_7(src, dst)						\
	(dst)[7] = (src)[7];

#define	ZERO_4(p)							\
	(p)[0] = 0;							\
	(p)[1] = 0;							\
	(p)[2] = 0;							\
	(p)[3] = 0;

#define	ZERO_6(p)							\
	ZERO_4(p)							\
	(p)[4] = 0;							\
	(p)[5] = 0;

#define	ZERO_8(p)							\
	ZERO_6(p)							\
	(p)[6] = 0;							\
	(p)[7] = 0;

#define GET_BIT(j, src, dst, pt)					\
	n = 7 - (pt)[j]%8;						\
	l = (src)[(pt)[j]/8];						\
	l &= 0x1 << n;							\
	l = (l >> n) & 0x1;						\
	(dst)[j/8] |= l << (7 - (j%8));

#define	PERMUTE_32(src, dst, pt)					\
	GET_BIT(0, src, dst, pt)					\
	GET_BIT(1, src, dst, pt)					\
	GET_BIT(2, src, dst, pt)					\
	GET_BIT(3, src, dst, pt)					\
	GET_BIT(4, src, dst, pt)					\
	GET_BIT(5, src, dst, pt)					\
	GET_BIT(6, src, dst, pt)					\
	GET_BIT(7, src, dst, pt)					\
	GET_BIT(8, src, dst, pt)					\
	GET_BIT(9, src, dst, pt)					\
	GET_BIT(10, src, dst, pt)					\
	GET_BIT(11, src, dst, pt)					\
	GET_BIT(12, src, dst, pt)					\
	GET_BIT(13, src, dst, pt)					\
	GET_BIT(14, src, dst, pt)					\
	GET_BIT(15, src, dst, pt)					\
	GET_BIT(16, src, dst, pt)					\
	GET_BIT(17, src, dst, pt)					\
	GET_BIT(18, src, dst, pt)					\
	GET_BIT(19, src, dst, pt)					\
	GET_BIT(20, src, dst, pt)					\
	GET_BIT(21, src, dst, pt)					\
	GET_BIT(22, src, dst, pt)					\
	GET_BIT(23, src, dst, pt)					\
	GET_BIT(24, src, dst, pt)					\
	GET_BIT(25, src, dst, pt)					\
	GET_BIT(26, src, dst, pt)					\
	GET_BIT(27, src, dst, pt)					\
	GET_BIT(28, src, dst, pt)					\
	GET_BIT(29, src, dst, pt)					\
	GET_BIT(30, src, dst, pt)					\
	GET_BIT(31, src, dst, pt)         

#define PERMUTE_48(src, dst, pt)					\
	PERMUTE_32(src, dst, pt)					\
	GET_BIT(32, src, dst, pt)					\
	GET_BIT(33, src, dst, pt)					\
	GET_BIT(34, src, dst, pt)					\
	GET_BIT(35, src, dst, pt)					\
	GET_BIT(36, src, dst, pt)					\
	GET_BIT(37, src, dst, pt)					\
	GET_BIT(38, src, dst, pt)					\
	GET_BIT(39, src, dst, pt)					\
	GET_BIT(40, src, dst, pt)					\
	GET_BIT(41, src, dst, pt)					\
	GET_BIT(42, src, dst, pt)					\
	GET_BIT(43, src, dst, pt)					\
	GET_BIT(44, src, dst, pt)					\
	GET_BIT(45, src, dst, pt)					\
	GET_BIT(46, src, dst, pt)					\
	GET_BIT(47, src, dst, pt)					\

#define	PERMUTE_56(src, dst, pt)					\
	PERMUTE_48(src, dst, pt)					\
	GET_BIT(48, src, dst, pt)					\
	GET_BIT(49, src, dst, pt)					\
	GET_BIT(50, src, dst, pt)					\
	GET_BIT(51, src, dst, pt)					\
	GET_BIT(52, src, dst, pt)					\
	GET_BIT(53, src, dst, pt)					\
	GET_BIT(54, src, dst, pt)					\
	GET_BIT(55, src, dst, pt)					\

#define	PERMUTE_64(src, dst, pt)					\
	PERMUTE_56(src, dst, pt)					\
	GET_BIT(56, src, dst, pt)					\
	GET_BIT(57, src, dst, pt)					\
	GET_BIT(58, src, dst, pt)					\
	GET_BIT(59, src, dst, pt)					\
	GET_BIT(60, src, dst, pt)					\
	GET_BIT(61, src, dst, pt)					\
	GET_BIT(62, src, dst, pt)					\
	GET_BIT(63, src, dst, pt)

#define	MAKE_INDEX(x)							\
	x = ((x & 0x01) << 4) |						\
	    ((x & 0x1e) >> 1) |						\
	     (x & 0x20);

#define	CMASK(N)	(0x3 >> (2-N))

#define	ROTATE_KEY(N)							\
	l = (k[0] >> (8-N)) & CMASK(N);						\
	k[0] <<= N;							\
	k[0] |= ((k[1] >> (8-N)) & CMASK(N));				\
	k[1] <<= N;							\
	k[1] |= ((k[2] >> (8-N)) & CMASK(N));				\
	k[2] <<= N;							\
	k[2] |= ((k[3] >> (8-N)) & CMASK(N));				\
	k[3] <<= N;							\
	l |= k[3];							\
	k[3] &= ~((char)(CMASK(N) << 4));				\
	k[3] |= ((l & CMASK(N)) << 4);					\
	k[3] |= ((k[4] >> (8-N)) & CMASK(N));				\
	k[4] <<= N;							\
	k[4] |= ((k[5] >> (8-N)) & CMASK(N));				\
	k[5] <<= N;							\
	k[5] |= ((k[6] >> (8-N)) & CMASK(N));				\
	k[6] <<= N;							\
	k[6] |= ((l >> 4) & CMASK(N));

#define	DES_ROUND							\
	ZERO_6(buf)							\
	PERMUTE_48(m+4, buf, des_ept)					\
									\
	buf[5] ^= key[5];						\
	buf[4] ^= key[4];						\
	buf[3] ^= key[3];						\
	buf[2] ^= key[2];						\
	buf[1] ^= key[1];						\
	buf[0] ^= key[0];						\
									\
	buf[7] =   buf[5] & 0x3f;					\
	buf[6] = ((buf[5] >> 6) & 0x03) | ((buf[4] << 2) & 0x3c);	\
	buf[5] = ((buf[4] >> 4) & 0x0f) | ((buf[3] << 4) & 0x30);	\
	buf[4] = ((buf[3] >> 2) & 0x3f);				\
	buf[3] =   buf[2] & 0x3f;					\
	buf[2] = ((buf[2] >> 6) & 0x03) | ((buf[1] << 2) & 0x3c);	\
	buf[1] = ((buf[1] >> 4) & 0x0f) | ((buf[0] << 4) & 0x30);	\
	buf[0] = ((buf[0] >> 2) & 0x3f);				\
									\
	MAKE_INDEX(buf[7])						\
	MAKE_INDEX(buf[6])						\
	MAKE_INDEX(buf[5])						\
	MAKE_INDEX(buf[4])						\
	MAKE_INDEX(buf[3])						\
	MAKE_INDEX(buf[2])						\
	MAKE_INDEX(buf[1])						\
	MAKE_INDEX(buf[0])						\
									\
	buf[0] = (des_sbox0[(int)buf[0]] << 4) | des_sbox1[(int)buf[1]];\
	buf[1] = (des_sbox2[(int)buf[2]] << 4) | des_sbox3[(int)buf[3]];\
	buf[2] = (des_sbox4[(int)buf[4]] << 4) | des_sbox5[(int)buf[5]];\
	buf[3] = (des_sbox6[(int)buf[6]] << 4) | des_sbox7[(int)buf[7]];\
									\
	ZERO_4(buf+4)							\
	PERMUTE_32(buf, buf+4, des_cpt)					\
									\
	((int *)buf)[0] = ((int *)buf)[1] ^ ((int *)m)[0];		\
	((int *)m)[0] = ((int *)m)[1];					\
	((int *)m)[1] = ((int *)buf)[0];				\

#define	ENC_ROUND(N, r)							\
	ROTATE_KEY(r)							\
	ZERO_6(key)							\
	PERMUTE_48(k, key, des_kcpt)					\
	DES_ROUND

#define	DEC_ROUND(N)							\
	COPY_6(keyring[N-1], key)					\
	DES_ROUND

#define	GET_SUBKEY(N, r)						\
	ROTATE_KEY(r)							\
	ZERO_6(keyring[N-1])						\
	PERMUTE_48(k, keyring[N-1], des_kcpt)

unsigned char des_ipt[64] = {
	57, 49, 41, 33, 25, 17,  9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7,
	56, 48, 40, 32, 24, 16,  8, 0,
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6
};

unsigned char des_fpt[64] = {
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41,  9, 49, 17, 57, 25,
	32, 0, 40,  8, 48, 16, 56, 24
};

unsigned char des_ept[48] = {
	31,  0,  1,  2,  3,  4,  3,  4,  5,  6,  7,  8,
	 7,  8,  9, 10, 11, 12, 11, 12, 13, 14, 15, 16,
	15, 16, 17, 18, 19, 20, 19, 20, 21, 22, 23, 24,
	23, 24, 25, 26, 27, 28, 27, 28, 29, 30, 31,  0
};

unsigned char des_kpt[56] = {
	56, 48, 40, 32, 24, 16,  8,  0, 57, 49, 41, 33, 25, 17,
	 9,  1, 58, 50, 42, 34, 26, 18, 10,  2, 49, 51, 43, 35,
	62, 54, 46, 38, 30, 22, 14,  6, 61, 53, 45, 37, 29, 21,
	13,  5, 60, 52, 44, 36, 28, 20, 12,  4, 27, 19, 11,  3
};

unsigned char des_cpt[32] = {
	15,  6, 19, 20, 28, 11, 27, 16,  0, 14, 22, 25,  4, 17, 30,  9,
	 1,  7, 23, 13, 31, 26,  2,  8, 18, 12, 29,  5, 21, 10,  3, 24
};

unsigned char des_kcpt[48] = {
	13, 16, 10, 23,  0,  4,  2, 27, 14,  5, 20,  9,
	22, 18, 11,  3, 25,  7, 15,  6, 26, 19, 12,  1,
	40, 51, 30, 36, 46, 54, 29, 39, 50, 44, 32, 47,
	43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31
};

unsigned char des_sbox0[64] = {
	14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
	 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
	 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
	15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13
};

unsigned char des_sbox1[64] = {
	15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
	 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
	 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
	13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9
};

unsigned char des_sbox2[64] = {
	10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
	13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
	13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
	 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12
};

unsigned char des_sbox3[64] = {
	 7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
	13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
	10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
	 3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14
};

unsigned char des_sbox4[64] = {
	 2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
	14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
	 4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
	11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3
};

unsigned char des_sbox5[64] = {
	12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
	10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
	 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
	 4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13
};

unsigned char des_sbox6[64] = {
	 4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
	13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
	 1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
	 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12
};

unsigned char des_sbox7[64] = {
	13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
	 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
	 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
	 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
};

void
des_block_encrypt(char *m, char *k)
{
	unsigned int n;
	unsigned char l;
	unsigned char key[6];
	unsigned char buf[8];

	ZERO_8(buf)
	PERMUTE_64(m, buf, des_ipt)
	COPY_8(buf, m)

	ZERO_8(buf)
	PERMUTE_56(k, buf, des_kpt)
	COPY_7(buf, k)

	ENC_ROUND(1, 1)
	ENC_ROUND(2, 1)
	ENC_ROUND(3, 2)
	ENC_ROUND(4, 2)
	ENC_ROUND(5, 2)
	ENC_ROUND(6, 2)
	ENC_ROUND(7, 2)
	ENC_ROUND(8, 2)
	ENC_ROUND(9, 1)
	ENC_ROUND(10, 2)
	ENC_ROUND(11, 2)
	ENC_ROUND(12, 2)
	ENC_ROUND(13, 2)
	ENC_ROUND(14, 2)
	ENC_ROUND(15, 2)
	ENC_ROUND(16, 1)

	COPY_4(m, buf)
	COPY_4(m+4, m)
	COPY_4(buf, m+4)

	ZERO_8(buf)
	PERMUTE_64(m, buf, des_fpt)
	COPY_8(buf, m)
}

void
des_block_decrypt(char *m, char *k)
{
	unsigned int n;
	unsigned char l;
	unsigned char key[6];
	unsigned char buf[8];
	unsigned char keyring[16][6];

	ZERO_8(buf)
	PERMUTE_64(m, buf, des_ipt)
	COPY_8(buf, m)

	ZERO_8(buf)
	PERMUTE_56(k, buf, des_kpt)
	COPY_7(buf, k)

	GET_SUBKEY(1, 1)
	GET_SUBKEY(2, 1)
	GET_SUBKEY(3, 2)
	GET_SUBKEY(4, 2)
	GET_SUBKEY(5, 2)
	GET_SUBKEY(6, 2)
	GET_SUBKEY(7, 2)
	GET_SUBKEY(8, 2)
	GET_SUBKEY(9, 1)
	GET_SUBKEY(10, 2)
	GET_SUBKEY(11, 2)
	GET_SUBKEY(12, 2)
	GET_SUBKEY(13, 2)
	GET_SUBKEY(14, 2)
	GET_SUBKEY(15, 2)
	GET_SUBKEY(16, 1)

	DEC_ROUND(16)
	DEC_ROUND(15)
	DEC_ROUND(14)
	DEC_ROUND(13)
	DEC_ROUND(12)
	DEC_ROUND(11)
	DEC_ROUND(10)
	DEC_ROUND(9)
	DEC_ROUND(8)
	DEC_ROUND(7)
	DEC_ROUND(6)
	DEC_ROUND(5)
	DEC_ROUND(4)
	DEC_ROUND(3)
	DEC_ROUND(2)
	DEC_ROUND(1)

	COPY_4(m, buf)
	COPY_4(m+4, m)
	COPY_4(buf, m+4)

	ZERO_8(buf)
	PERMUTE_64(m, buf, des_fpt)
	COPY_8(buf, m)
}

