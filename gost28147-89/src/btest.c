#include <stdio.h>

#include "gost89.h"

int
main()
{
	char m[] = "gost89!";
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

	printf("m = %s\n", m);
	gost89_block_encrypt(m, (char *)k);
	printf("m = %x %x\n", *((int *)m), *((int *)(m+1)) );
	gost89_block_decrypt(m, (char *)k);
	printf("m = %s\n", m);

	return 0;
}

