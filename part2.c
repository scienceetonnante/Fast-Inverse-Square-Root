
// Print the bit representation of a float, takes as an argument a 32 bit integer
// obtained using a cast from a float pointer like this:
// int i = * ( int * ) &f;
void print_bits(int i) {

    char bit_string[36];    // 32 bits + end of string + 3 spaces for separating sign, exponent, and mantissa

    int sign = (i >> 31) & 1;
    int exponent = (i >> 23) & 0xFF;
    int mantissa = i & 0x7FFFFF;

    int cnt = 0;
    for(int index = 31; index >= 0; --index)
    {
        if (index == 30 || index == 22)
        {
            bit_string[cnt++] = ' ';
        }
        bit_string[cnt++] = (i & (1 << index)) ? '1' : '0';
    }
    bit_string[35] = '\0';

    float f = * ( float * ) &i;
    printf("%s | Sign: %d, Exp: %d, Mant: %d Repr. %e\n", bit_string, sign, exponent, mantissa, f);
}

// APPROXIMATE INVERSE
// Takes a 32 bit integer, interpret it as a float, and approximate the inverse by negating the exponent
// - Don't touch the last 23 bits (mantissa)
// - Don't touch the first bit (sign bit)
// - Replace the next 8 bits (representing a char E between 0 and 255) by the 8 bits representing 254 - E
// With this, if we consider a float 2^P, its char exponent part will be 127 + P.
// To get it to -P represented by 127 - P, we have to do
// 254 - (127 + P) = 127 - P
// Hence the magic constant being 254 * 2^23
// ie 2130706432 ie bitwise 0 11111110 00000000000000000000000 ie 0x7F000000
float approximate_inverse(float a)
{
    printf("APPROXIMATE INVERSE OF %f\n",a);
    int i = * ( int * ) &a;

    printf("Initial bit representation:   ");
    print_bits(i);

    i = - i;
    printf("After negation:               ");
    print_bits(i);

    // Magic constant 254 * 2^23
    int magic_constant = 0x7F000000;
    printf("Magic constant:               ");
    print_bits(magic_constant);

    i = i + magic_constant;
    printf("After adding magic constant:  ");
    print_bits(i);

    printf("\n");
    return * ( float * ) &i;
}

// APPROXIMATE SQUARE ROOT
// Takes a 32 bit integer, interpret it as a float, and approximate the square root by halving the exponent
// Applying a right shift will halve the exponent, but we need to take care of the 127 bias.
// For instance for 2^P (say P is even), the exponent is 127 + P.
// Right shifting it will give 63 + P/2, which is not what we want, we want 127 + P/2.
// To get it to 127 + P/2, we have to add the magic constant 127/2 * 2^23

float approximate_sqrt(float a)
{
    printf("APPROXIMATE SQUARE ROOT OF %f\n",a);
    int i = * ( int * ) &a;

    printf("Initial bit representation:   ");
    print_bits(i);

    i = i >> 1;
    printf("After right shift:            ");
    print_bits(i);

    // Magic constant 127/2 * 2^23 ie 532676608
    int magic_constant = 0x1FC00000;
    printf("Magic constant:               ");
    print_bits(magic_constant);

    i = i + magic_constant;
    printf("After adding magic constant:  ");
    print_bits(i);

    printf("\n");
    return * ( float * ) &i;
}

// APPROXIMATE INVERSE SQUARE ROOT
// Takes a 32 bit integer, interpret it as a float, and approximate the inverse square root by halving the exponent
// and adding a magic constant
// For instance for 2^P (say P is even), the exponent is 127 + P, we want to get 127 - P/2
// To get from (127 + P)/2 to 127 - P/2, we have to do (3/2)*127 - (127 + P)/2
// So the magic constant is (3/2)*127 * 2^23 ie 0x5F400000
// Now it can be slightly optimized by using the fact that log2(1 + x) = x + SHIFT in the [0,1] interval
// where SHIFT is a slight correction being roughly 0.043
// So the better magic constant is (3/2) * (127 - SHIFT) * 2^23 ie 0x5f375a86
float approximate_invsqrt(float a, int use_quake3_magic_constant)
{
    printf("APPROXIMATE INVERSE SQUARE ROOT OF %f\n",a);
    int i = * ( int * ) &a;

    printf("Initial bit representation:   ");
    print_bits(i);

    i = i >> 1;
    printf("After right shift:            ");
    print_bits(i);

    i = - i;
    printf("After negation:               ");
    print_bits(i);

    // Magic constant (3/2) * 127 * 2^23 is x5F400000 ie 1598029824
    // In the quake3 version, the magic constant is 0x5f375a86 ie (3/2) * (127 - SHIFT) * 2^23
    // where SHIFT is a slight correction : log2(1 + x) = x + SHIFT in the [0,1] interval
    int magic_constant = 0x5F400000;
    if (use_quake3_magic_constant) magic_constant = 0x5f375a86;
    printf("Magic constant:               ");
    print_bits(magic_constant);

    i = magic_constant + i;
    printf("After adding magic constant:  ");
    print_bits(i);

    printf("\n");
    return * ( float * ) &i;
}


void part2()
{
    approximate_inverse(8);
    approximate_inverse(12);

    approximate_sqrt(16);
    approximate_sqrt(32);

    approximate_invsqrt(0.25f,0);
    approximate_invsqrt(0.125f,0);

    approximate_invsqrt(0.25f,1);
    approximate_invsqrt(0.125f,1);
}
