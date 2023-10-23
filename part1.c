#include <stdio.h>
#include <math.h>

// The original code, verbatim
float Q_rsqrt( float number )
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;						// evil floating point bit level hacking

    i  = 0x5f3759df - ( i >> 1 );               // what the fuck?

    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
//	y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

    return y;
}


// Clearer code, using int instead of long
float myQ_rsqrt( float a )
{
    int i  = * ( int * ) &a;					// evil floating point bit level hacking
    i  = 0x5f3759df - ( i >> 1 );               // what the fuck?
    float x  = * ( float * ) &i;

    x  = x * (1.5F - 0.5F * a * x * x );   // 1st iteration
//	x  = x * (1.5F - 0.5F * a * x * x );   // 2nd iteration, this can be removed

    return x;
}


// Newton's method for square root
float sqrt_newton(float a)
{
    printf("Exact: %f\n",sqrtf(a));

    float x = a;     // Initial guess
    float fx = x * x - a;

    const float TOL = 1e-4f;
    const int MAX_ITER = 20;
    for(int niter = 0; (niter < MAX_ITER) && (fabsf(fx) > TOL); niter++)
    {
        x += - fx / (2 * x);
        fx = x * x - a;

        printf("%f\n", x);
    }
    printf("\n");
    return x;
}


// Newton's method for inverse square root
float invsqrt_newton(float a)
{
    printf("Exact: %f\n",1.F/sqrtf(a));

    float x = 1/a;     // Initial guess

    const int MAX_ITER = 20;
    for(int niter = 0; niter < MAX_ITER; niter++)
    {
        x = x * (1.5F - 0.5F * a * x * x);

        printf("%f\n", x);
    }
    printf("\n");
    return x;
}

void part1()
{
    float test_number = 0.345f;
    sqrt_newton(test_number);
    invsqrt_newton(test_number);

    printf("Q_sqrt : %f\n", Q_rsqrt(test_number));
    printf("myQ_sqrt : %f\n", myQ_rsqrt(test_number));
}