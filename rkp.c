/*==================================================================================================
	[rkp]
		2015.02.13
	[Input]
		$./rkp (l: weight limit) (t: number of trials)
	[Output]
		data.
==================================================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <time.h>
#include "mertwi.h"

/* repeat count. */ 
#define K 20

int calc_v(int n, double complex *a, int *r)
{
	int i;
	int value;
	
	value = 0;
	
	for ( i = 0; i < n; i++ )
		value += creal(a[i]) * r[i];
	
	return value;
}

int calc_w(int n, double complex *a, int *r)
{
	int i;
	int weight;
	
	weight = 0;
	
	for ( i = 0; i < n; i++ )
		weight += cimag(a[i]) * r[i];
		
	return weight;
}

/* knapsack problem. */
void knapsack(int n, int t, int l, double complex *a, int *r)
{
	int i, j;
	int value, weight;
	int m_value, m_weight;
	
	m_value = 0;
	m_weight = 0;
	
	for ( i = 0; i < t; i++ ) {
		/* Choose combination. */
		for ( j = 0; j < n; j++ )
			r[j] = genrand_real3()*2;
		
		/* Compute value and weight. */
		value = calc_v(n, a, r);
		weight = calc_w(n, a, r);
		
		if ( weight > l ) continue;
		
		/* Update max value and max weight. */
		if ( value > m_value ) {
			m_value = value;
			m_weight = weight;
		}
	}
	
	/* Result. */
	printf("%d %d ", m_value, m_weight);
	for ( i = 0; i < n; i++ ) {
		printf("%d", r[i]);
		if ( i == n ) printf("\n");
	}
}

main(int argc, char **argv)
{
	FILE *in;
	int *r;
	int i, n, l, t, size;
	double complex *a;
	
	/* Input data. */
	l = atoi(argv[1]);
	t = atoi(argv[2]);
	in = fopen(argv[3], "rb");
	fseek(in, 0, SEEK_END);
	size = ftell(in);
	n = size / sizeof(complex double);
	rewind(in);
	a = (double complex *)malloc(n * sizeof(double complex));
	fread(a, n, sizeof(double complex), in);
	fclose(in);
	
	r = (int *)malloc(n * sizeof(int));
	init_genrand((int)time( NULL ));
	
	/* Output data. */
	printf("items           :%d\n", n);
	printf("weight limit    :%d\n", l);
	printf("trial counts    :%d\n", t);
	printf("-----\n");
	for ( i = 0; i < K; i++ ) knapsack(n, t, l, a, r);
}
