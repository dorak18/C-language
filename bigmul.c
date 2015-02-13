/*==================================================================================================
	[bigmul]
		2015.02.03
	[Input]
		a: (a_0, ..., a_N-1, 0, ..., 0),
		b: (b_0, ..., b_N-1, 0, ..., 0).
	[Output]
		a * b.
==================================================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h> 

/* bit reverse: (a_0, ..., a_N-1) -> (a_N-1, ..., a_0) */
void bitreverse(int l,double complex *a)
{
	int i, n;
	unsigned int b, r;
	double complex t;
	
	n = 1 << l;
	
	for ( r = 0, i = 0; i < n; i++ ) {
		/* Swap i-th data and r-th data. */
		if ( i < r ) {
			t = a[i];
			a[i] = a[r];
			a[r] = t;
		}
		
		b = 1 << (l - 1);
		while ( b >= 1 && r >= b ) {
			r -= b;
			b >>= 1;
		}
		r += b;
	}
}

void fft(int l, double complex *a, int idft)
{
	int i, j, k, N;
	double complex r, u, v, alpha, beta;
	
	bitreverse(l, a);
	N = 1 << l;
	alpha = -1;
	i = 2;
	
	while ( i <= N ) {
		for ( j = 0; j < N; j += i ) {
			beta = 1;
			for ( k = 0; k < i/2; k++ ) {
				u = a[j+k];
				v = a [j+k+(i/2)];
				a[j+k] = u + beta * v;
				a[j+k+(i/2)] = u - beta * v;
				beta = alpha * beta;
			}
		}
		
		if ( i == 2 ) {
			if ( idft )
				alpha = I;
			else
				alpha = -I;
		} else {
			r = sqrt((1+creal(alpha))/2);
			alpha = r + I*(cimag(alpha)/(2*r));
		}
		i *= 2;
	}
	
	if ( idft )
		for ( i = 0; i < N; i++ ) a[i] /= N;
}

void printz(int n, double complex *a, int *r)
{
	int i, c;
	
	/* Carry over. */
	for ( c = 0, i = n-1; i >= 0; i-- ) {
		c += creal(a[i]) + 0.5;
		r[i+1] = c % 100;
		c /= 100;
	}
	
	/* Output data. */
	printf("Result: ");
	for ( r[0] = c, i = 0; i <  n; i++ ) {
		printf("%02d ", r[i]);
	}
}

main(int argc,char **argv)
{
	FILE *in;
	int *r;
	int i, l, n, b, size;
	double complex *a1, *a2;
	
	/* Input data. */
	in = fopen(argv[1], "rb");
	fseek(in, 0, SEEK_END);
	size = ftell(in);
	n = size / sizeof(complex double);
	for ( l = 0, b = 1; b < n; b <<= 1, l++ );
	if ( n != b ) {
		printf( "size n must be a power of 2.\n" );
		exit(1);
	}
	rewind(in);
	a1 = (double complex *)calloc(n, sizeof(double complex));
	fread(a1, n, sizeof(double complex), in);
	fclose(in);
	in = fopen(argv[2], "rb");
	a2 = (double complex *)calloc(n, sizeof(double complex));
	fread(a2, n, sizeof(double complex), in);
	fclose(in);
	
	/* Work space. */
	r = (int *)malloc(n * sizeof(int));
	
	/* DFT */
	fft(l, a1, 0);
	fft(l, a2, 0);
	for ( i = 0; i < n; i++ ) a1[i] *= a2[i];
	/* iDFT */
	fft(l, a1, 1);
	
	printz(n, a1, r);
}
