#include <stdio.h>

#include "des.h"

int
main()
{
	int i;
	char m[] = "password";
	char k[] = {
/*		0x01, 0x23, 0x45, 0x67,
		0x89, 0xab, 0xcd, 0xef

		0x00, 0xff, 0x00, 0xff,
		0xff, 0x00, 0xff, 0x00
*/
		0xde, 0xad, 0xbe, 0xef,
		0xde, 0xad, 0xbe, 0xef
	};
	char tmp[8];

	printf("m = %s\n", m);

	for (i = 0; i < 8; i++)
		tmp[i] = k[i];

	des_block_encrypt(m, tmp);

	printf("m = ");
	for (i = 0; i < 8; i++)
		printf("%x ", (int)m[i] & 0xff);
	printf("\n");

	for (i = 0; i < 8; i++)
		tmp[i] = k[i];

	des_block_decrypt(m, tmp);

	printf("m = %s\n", m);


	return 0;
}

