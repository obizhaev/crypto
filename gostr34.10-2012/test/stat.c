#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#define	SIZE	1024

int
main()
{
	FILE *fp;
	int hash[SIZE];
	int pmul[SIZE];
	int math[SIZE];
	char buf[16];
	long int avg;
	int hn, pn, mn;
	int i;

	fp = fopen("./measures", "r");
	if (fp == NULL) {
		perror("can't open file");
		return 1;
	}

	hn = pn = mn = 0;
	while (fgets(buf, 16, fp) != NULL) {
		hash[hn] = atoi(buf);
		hn++;

		if (fgets(buf, 16, fp) == NULL)
			break;
		pmul[pn] = atoi(buf);
		pn++;

		if (fgets(buf, 16, fp) == NULL)
			break;
		math[mn] = atoi(buf);
		mn++;
	}

	if (hn != 0) {
		avg = 0;
		for (i = 0; i < hn; i++)
			avg += hash[i];
		avg /= hn;
		printf("hash:\t\t%li microsecs\n", avg);
	}

	if (pn != 0) {
		avg = 0;
		for (i = 0; i < pn; i++)
			avg += pmul[i];
		avg /= pn;
		printf("point mul:\t%li microsecs\n", avg);
	}

	if (mn != 0) {
		avg = 0;
		for (i = 0; i < mn; i++)
			avg += math[i];
		avg /= mn;
		printf("all maths:\t%li microsecs\n", avg);
	}

	fclose(fp);

	return 0;
}

