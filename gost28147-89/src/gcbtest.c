#include <stdio.h>

#include "gost89.h"

int
main()
{
	int i;
	char m[] = "gost89 gamming callback algorythm";

	int k[] = {
		0xffffffff,
		0xdeadbeef,
		0xffffffff,
		0xdeadbeef,
		0xffffffff,
		0xdeadbeef,
		0xdeadbeef,
		0xffffffff
	};

	int s[] = {
		0x87654321,
		0x12345678
	};

	int buf[] = {
		0x87654321,
		0x12345678
	};

	printf("message size is %i\n", (int)sizeof(m));
	printf("m = %s\n", m);

	gost89_gamming_cb_encrypt(m, sizeof(m), (char *)k, (char *)s);

	printf("m = ");
	for (i = 0; i < sizeof(m); i++)
		printf("%x ", ((int)m[i]) & 0xff);
	printf("\n");

	s[0] = buf[0];
	s[1] = buf[1];

	gost89_gamming_cb_decrypt(m, sizeof(m), (char *)k, (char *)s);

	printf("m = %s\n", m);

	return 0;
}

