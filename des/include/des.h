#ifndef	DES_H_
#define	DES_H_

int des_key_transform(const unsigned char *src, unsigned char *dst, int n);

void des_block_encrypt(char *m, char *k);

void des_block_decrypt(char *m, char *k);

int des_cbc_encrypt(char *m, int n, char *k, char *s);

int des_cbc_decrypt(char *m, int n, char *k, char *s);

#endif

