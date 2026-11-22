/*
 * Task 09 - Alternative concept: slab limits and rates kept in two ARRAYS and
 * the bill computed by a loop over the slabs, instead of fixed if statements.
 * Checks that it gives the same bill as compute_bill() for every unit count and
 * measures the time per bill. Adding a slab = adding one array element.
 */
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include "billing.h"

#define SLABS 3
static const unsigned int upper_limit[SLABS] = {SLAB1_UNITS, SLAB2_UNITS, UINT_MAX};
static const long         rate[SLABS]        = {RATE1_PAISE, RATE2_PAISE, RATE3_PAISE};

static long compute_bill_loop(unsigned int units)
{
    long paise = FIXED_CHARGE_PAISE;
    unsigned int lower = 0;
    for (int s = 0; s < SLABS && units > lower; s++) {          /* O(s) */
        unsigned int top  = units < upper_limit[s] ? units : upper_limit[s];
        paise += (long)(top - lower) * rate[s];
        lower = upper_limit[s];
    }
    return paise;
}

int main(void)
{
    int mismatches = 0;
    for (unsigned int u = 0; u <= MAX_MONTHLY_UNITS; u++)
        if (compute_bill_loop(u) != compute_bill(u)) mismatches++;
    printf("units 0..%u compared: %d mismatches between loop version and if-version\n", MAX_MONTHLY_UNITS, mismatches);

    const long N = 100000000L;
    long sum1 = 0, sum2 = 0;
    clock_t t0 = clock();
    for (long i = 0; i < N; i++) sum1 += compute_bill((unsigned int)(i % 400));
    clock_t t1 = clock();
    for (long i = 0; i < N; i++) sum2 += compute_bill_loop((unsigned int)(i % 400));
    clock_t t2 = clock();
    printf("fixed if-statements : %.3f ns per bill (sum %ld)\n", 1e9 * (double)(t1 - t0) / CLOCKS_PER_SEC / N, sum1);
    printf("loop over %d slabs   : %.3f ns per bill (sum %ld)\n", SLABS, 1e9 * (double)(t2 - t1) / CLOCKS_PER_SEC / N, sum2);
    printf("extra space (two arrays): %zu bytes\n", sizeof upper_limit + sizeof rate);
    return mismatches != 0;
}
