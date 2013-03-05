#include "des.h"

int
des_cbc_encrypt(char *m, int n, char *k, char *s)
{
	int i;
	int syn[2];

	if (n%8 != 0)
		return -1;

	syn[0] = ((int *)s)[0];
	syn[1] = ((int *)s)[1];

	for (i = 0; i < n/8; i++) {
		((int *)m)[0] ^= ((int *)syn)[0];
		((int *)m)[1] ^= ((int *)syn)[1];

		des_block_encrypt(m, k);

		((int *)syn)[0] = ((int *)m)[0];
		((int *)syn)[1] = ((int *)m)[1];

		m += 8;
	}

	return 0;
}

int
des_cbc_decrypt(char *m, int n, char *k, char *s)
{
	int i;
	int syn[2];

	if (n%8 != 0)
		return -1;

	for (i = 0; i < n/8; i++) {
		syn[0] = ((int *)m)[0];
		syn[1] = ((int *)m)[1];

		des_block_decrypt(m, k);

		((int *)m)[0] ^= ((int *)s)[0];
		((int *)m)[1] ^= ((int *)s)[1];

		((int *)s)[0] = syn[0];
		((int *)s)[1] = syn[1];

		m += 8;
	}

	return 0;
}

