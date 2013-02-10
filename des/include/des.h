#ifndef	DES_H_
#define	DES_H_

#define DES_ENCRYPT	0
#define DES_DECRYPT	1

enum {
	DES_OK	= 0,
	DES_ERR
};

int des_key_transform(const unsigned char *src, unsigned char *dst, int n);

void des_block_encrypt(char *m, char *k);

void des_block_decrypt(char *m, char *k);

#endif

