#include <stdio.h>

#define	CMASK(n)	(0x3 >> (2-n))

#define	ROTATE_KEY(N)							\
	l = (k[0] >> (8-N)) & CMASK(N);						\
	k[0] <<= N;							\
	k[0] |= ((k[1] >> (8-N)) & CMASK(N));				\
	k[1] <<= N;							\
	k[1] |= ((k[2] >> (8-N)) & CMASK(N));				\
	k[2] <<= N;							\
	k[2] |= ((k[3] >> (8-N)) & CMASK(N));				\
	k[3] <<= N;							\
	printf("l = %x\n", ((int)l) & 0xff );							\
	l |= k[3];							\
	printf("l = %x\n", ((int)l) & 0xff );							\
	k[3] &= ~((char)(CMASK(N) << 4));				\
	k[3] |= ((l & CMASK(N)) << 4);					\
	k[3] |= ((k[4] >> (8-N)) & CMASK(N));				\
	k[4] <<= N;							\
	k[4] |= ((k[5] >> (8-N)) & CMASK(N));				\
	k[5] <<= N;							\
	k[5] |= ((k[6] >> (8-N)) & CMASK(N));				\
	k[6] <<= N;							\
	printf("l = %x\n", ((int)l) & 0xff );							\
	k[6] |= ((l >> 4) & CMASK(N));

int
main()
{
	int i;
	char l;
	char k[] = {
		0x00, 0x80, 0x80,
		0x88,
		0x80, 0x80, 0x80
	};

	for (i = 0; i < 7; i++)
		printf("%x ", (int)(k[i]) & 0xff);
	printf("\n");

	ROTATE_KEY(1)

	for (i = 0; i < 7; i++)
		printf("%x ", (int)(k[i]) & 0xff);
	printf("\n");


	return 0;
}

