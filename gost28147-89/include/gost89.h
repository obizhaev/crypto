#ifndef GOST89_H_
#define GOST89_H_

char sbox[128];

void gost89_block_encrypt(char *m, const char *k);

void gost89_block_decrypt(char *m, const char *k);

void gost89_gamming_encrypt(char *m, int n, const char *k, char *s);

void gost89_gamming_decrypt(char *m, int n, const char *k, char *s);

void gost89_gamming_cb_encrypt(char *m, int n, const char *k, char *s);

void gost89_gamming_cb_decrypt(char *m, int n, const char *k, char *s);

#endif
