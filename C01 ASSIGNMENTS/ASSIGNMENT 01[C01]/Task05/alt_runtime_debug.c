/*
 * Task 05 - Alternative concept: debugging controlled by a RUN-TIME variable
 * (if (debug) printf(...)) instead of #ifdef DEBUG.
 * Run with -d to turn the debug output on. The debug code is always in the executable.
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "billing.h"

static int debug = 0;

static long compute_bill_rt(int units)
{
    long paise = FIXED_CHARGE_PAISE;
    if (debug) printf("[DEBUG] units=%d, start with fixed charge=%ld paise\n", units, paise);
    if (units > SLAB2_UNITS) {
        long part = (long)(units - SLAB2_UNITS) * RATE3_PAISE;
        paise += part;
        if (debug) printf("[DEBUG]   slab 3: %d units x %ld = %ld paise\n", units - SLAB2_UNITS, RATE3_PAISE, part);
    }
    if (units > SLAB1_UNITS) {
        int upper = units < SLAB2_UNITS ? units : SLAB2_UNITS;
        long part = (long)(upper - SLAB1_UNITS) * RATE2_PAISE;
        paise += part;
        if (debug) printf("[DEBUG]   slab 2: %d units x %ld = %ld paise\n", upper - SLAB1_UNITS, RATE2_PAISE, part);
    }
    paise += (long)(units < SLAB1_UNITS ? units : SLAB1_UNITS) * RATE1_PAISE;
    if (debug) printf("[DEBUG]   total = %ld paise\n", paise);
    return paise;
}

int main(int argc, char *argv[])
{
    if (argc > 1 && strcmp(argv[1], "--bench") == 0) {
        const long N = 100000000L;
        long sum = 0;
        clock_t t0 = clock();
        for (long i = 0; i < N; i++) sum += compute_bill_rt((int)(i % 400));
        clock_t t1 = clock();
        printf("bench: sum=%ld, %.3f ns per bill\n", sum, 1e9 * (double)(t1 - t0) / CLOCKS_PER_SEC / N);
        return 0;
    }
    if (argc > 1 && strcmp(argv[1], "-d") == 0) debug = 1;

    int tests[] = {0, 150, 350};
    for (int i = 0; i < 3; i++) {
        long p = compute_bill_rt(tests[i]);
        printf("%3d units -> Rs %ld.%02ld\n", tests[i], p / 100, p % 100);
    }
    return 0;
}
