/* Task 08 - small billing program used to follow every stage of compilation. */
#include <stdio.h>

#define FIXED_CHARGE_PAISE 5000L          /* Rs 50 fixed charge */
#define SLAB1_UNITS        100
#define SLAB2_UNITS        200
#define RATE2_PAISE        225L
#define RATE3_PAISE        450L
#define SQUARE_PAISE(x)    ((x) * 100L)   /* macro: replaced by the preprocessor */

long compute_bill(int units)              /* comment: removed by the preprocessor */
{
    long paise = FIXED_CHARGE_PAISE;
    if (units > SLAB2_UNITS) paise += (long)(units - SLAB2_UNITS) * RATE3_PAISE;
    if (units > SLAB1_UNITS) {
        int upper = units < SLAB2_UNITS ? units : SLAB2_UNITS;
        paise += (long)(upper - SLAB1_UNITS) * RATE2_PAISE;
    }
    return paise;
}

int main(void)
{
    int tests[] = {0, 150, 350};
    for (int i = 0; i < 3; i++) {
        long p = compute_bill(tests[i]);
        printf("%3d units -> Rs %ld.%02ld   (rupees to paise macro: %ld)\n",
               tests[i], p / 100, p % 100, SQUARE_PAISE(p / 100));
    }
    return 0;
}
