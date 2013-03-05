#ifndef	GOST_H_
#define	GOST_H_

int gost_hash_256(const char *msg, int n, char *buf);

int gost_hash_512(const char *msg, int n, char *buf);

#endif

