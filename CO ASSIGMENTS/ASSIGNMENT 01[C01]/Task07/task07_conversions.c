/*
 * Task 07 - Control type conversion and precision.
 * Seven expressions that give an unexpected result, each explained, followed
 * by the single rounding rule adopted for the project (integer round-half-up).
 * NOTE: gcc -Wall -Wextra warns about the signed/unsigned comparison in
 * surprise 3 - that warning is part of the evidence.
 */
#include <stdio.h>
#include <math.h>

/* Rounding rule of the project: divide in INTEGER arithmetic, half rounds up. */
static long round_div(long numerator, long denominator)
{
    return (numerator + denominator / 2) / denominator;
}

int main(void)
{
    /* 1. integer division */
    int units = 150;
    printf("1. integer division\n");
    printf("   150 / 100        = %d       (both operands int: fraction .5 is discarded)\n", units / 100);
    printf("   150 / 100.0      = %.2f    (one operand double: the int is converted to double)\n", units / 100.0);
    long paise = 16250;
    printf("   16250 paise / 100 = %ld rupees, remainder %ld paise (use / and %% together)\n", paise / 100, paise % 100);

    /* 2. narrowing a long to int */
    long big = 5000000000L;
    printf("2. cast of a large long to int\n");
    printf("   (int)5000000000L = %d   (value does not fit in 32 bits; the high bits are lost)\n", (int)big);

    /* 3. usual arithmetic conversions: signed compared with unsigned */
    int minus_one = -1;
    unsigned int one = 1u;
    printf("3. signed versus unsigned comparison\n");
    printf("   (-1 < 1u) = %d       (-1 is converted to unsigned 4294967295, so the test is false)\n", minus_one < one);

    /* 4. integer promotion of a 16-bit register */
    unsigned short reg = 65535;
    printf("4. integer promotion\n");
    printf("   reg + 1 = %d, but reg after reg++ = ", reg + 1);
    reg++;
    printf("%hu   (the sum is computed as int, the increment is stored back as 16-bit)\n", reg);

    /* 5. float arithmetic is not decimal arithmetic */
    volatile double a = 0.1, b = 0.2;
    printf("5. binary floating point\n");
    printf("   0.1 + 0.2 == 0.3 is %s; 0.1 + 0.2 = %.17g\n", (a + b == 0.3) ? "true" : "FALSE", a + b);

    /* 6. converting a fractional rupee amount to paise */
    volatile double rupees = 1.005;
    printf("6. converting rupees to paise\n");
    printf("   1.005 * 100 = %.17g -> (long) = %ld, llround = %lld   (the decimal rule says 101)\n",
           rupees * 100, (long)(rupees * 100), llround(rupees * 100));
    printf("   integer rule: round_div(1005, 10) = %ld\n", round_div(1005, 10));

    /* 7. integer promotion: long multiplication done in int overflows first */
    int u = 50000, rate = 450;
    long wrong = (long)(u * rate);          /* multiplied as int (22,500,000 fits...) */
    int  u2 = 5000000;
    long wrong2 = (long)(u2 * rate);        /* 2,250,000,000 does NOT fit in 32-bit int */
    long right2 = (long)u2 * rate;          /* cast first, then multiply in long */
    printf("7. cast placed after the multiplication instead of before\n");
    printf("   (long)(50000 * 450)   = %ld   (fits in int, so no harm)\n", wrong);
    printf("   (long)(5000000 * 450) = %ld   (product overflowed in int before the cast)\n", wrong2);
    printf("   (long)5000000 * 450   = %ld   (correct)\n", right2);

    /* Rounding rule applied: Wh x paise-per-kWh / 1000 */
    printf("\nRounding rule: whole-paise result = (numerator + denominator/2) / denominator in integers\n");
    long wh = 1235, rate_kwh = 450;
    long num = wh * rate_kwh;               /* 555,750 milli-paise */
    printf("   1235 Wh at 450 paise/kWh = %ld/1000 paise: truncated = %ld, rounded half-up = %ld\n",
           num, num / 1000, round_div(num, 1000));
    return 0;
}
