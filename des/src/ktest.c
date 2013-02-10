#include <stdio.h>

#include "des.h"

int
main()
{
	int i;
	unsigned char key[] = "asdasda";

	for (i = 0; i < 8; i++)
		printf("%x ", key[i]);
	printf("\n");

	des_key_transform(key, key, 64);

	for (i = 0; i < 8; i++)
		printf("%x ", key[i]);
	printf("\n");

	des_key_transform(key, key, 56);

	for (i = 0; i < 8; i++)
		printf("%x ", key[i]);
	printf("\n");

	return 0;
}

