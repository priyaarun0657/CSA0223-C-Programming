/*
 * Task 03 - Alternative concept: tariff constants as enum and const variables
 * instead of #define. Same bills, but the constants now have a type, a scope,
 * (for const) an address and a name that a debugger can see.
 */
#include <stdio.h>

enum { SLAB1_UNITS = 100, SLAB2_UNITS = 200 };          /* integer constants, no storage */
static const long RATE1_PAISE = 0;                       /* typed, scoped, occupy storage */
static const long RATE2_PAISE = 225;
static const long RATE3_PAISE = 450;
static const long FIXED_CHARGE_PAISE = 5000L;

/* enum constants are true constant expressions, so they may size arrays and label cases;
 * a "const int" is NOT a constant expression in C and could not be used here. */
static int slab_hits[SLAB2_UNITS / SLAB1_UNITS + 1];

static int slab_of(int units)
{
    switch (units / SLAB1_UNITS) {         /* uses the enum value in an expression */
    case 0:  return 0;
    case 1:  return 1;
    default: return 2;
    }
}

static long compute_bill(int units)
{
    long paise = FIXED_CHARGE_PAISE;
    if (units > SLAB2_UNITS) paise += (long)(units - SLAB2_UNITS) * RATE3_PAISE;
    if (units > SLAB1_UNITS) {
        int upper = units < SLAB2_UNITS ? units : SLAB2_UNITS;
        paise += (long)(upper - SLAB1_UNITS) * RATE2_PAISE;
    }
    paise += (long)(units < SLAB1_UNITS ? units : SLAB1_UNITS) * RATE1_PAISE;
    slab_hits[slab_of(units) > 2 ? 2 : slab_of(units)]++;
    return paise;
}

int main(void)
{
    int tests[] = {0, 100, 150, 200, 350};
    int n = sizeof tests / sizeof tests[0];
    for (int i = 0; i < n; i++) {
        long p = compute_bill(tests[i]);
        printf("%3d units -> Rs %ld.%02ld\n", tests[i], p / 100, p % 100);
    }
    printf("\nWhat is different from #define:\n");
    printf("  sizeof(RATE2_PAISE) as const long = %zu bytes   (a #define 225 is an int literal: %zu bytes)\n",
           sizeof RATE2_PAISE, sizeof(225));
    const long *addr = &RATE2_PAISE;     /* legal only for a variable, not for a #define */
    printf("  const variable has an address     = %s\n", addr ? "yes (visible in debugger)" : "no");
    printf("  array sized by enum constant      = %zu elements\n", sizeof slab_hits / sizeof slab_hits[0]);
    return 0;
}
