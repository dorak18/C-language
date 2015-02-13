/*==================================================================================================
	[bignum]
		2015.02.12
	[Input]
		a, n: natural number.
	[Output]
		a^n.
==================================================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>

#define K 7
#define X 10000000

void power_n(int a, int n, long *r)
{
	int c;
	int i, j;
	
	/* Initial value. */
	r[0] = 1;
	
	for ( i = 1; i <= n; i++ ) {
		c = 0;
		for ( j = 0; j < K; j++ ) {
			r[j] *= a;
			r[j] += c;
			
			/* Carry over */
			if ( r[j] >= X ) {
				c = r[j] / X;
				r[j] %= X;
			} else {
				c = 0;
			}
		}
		
		/* Output data. */
		printf("n=%3d: ", i);
		if ( r[K-1] && c ) {
				printf("over capacity.\n");
				exit(0);
		}
		for ( j = K-1; j >= 0; j-- ) {
			if ( r[j] == 0 )
				printf("        ");
			if ( r[j+1] == 0 || j == K-1 )
				printf("%7lu ", r[j]);
			
			printf("%07lu ", r[j]);
		}
		printf("\n");
	}
}

main(int argc, char **argv)
{
	int a, n;
	long *r;
	
	/* input data */
	a = atoi(argv[1]);
	n = atoi(argv[2]);
	
	/* workspace */
	r = (long *)calloc(K, sizeof(long));
	
	if ( a <= 0 || n <= 0 ) {
		fprintf(stderr, "! input error.\n");
		exit(1);
	}
	
	power_n(a, n, r);
}
