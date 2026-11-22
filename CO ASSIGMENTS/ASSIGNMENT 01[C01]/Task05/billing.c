#include <stdio.h>
#include "billing.h"

long compute_bill(int units)
{
    long paise = FIXED_CHARGE_PAISE;
#ifdef DEBUG
    printf("[DEBUG] units=%d, start with fixed charge=%ld paise\n", units, paise);
#endif
    if (units > SLAB2_UNITS) {
        long part = (long)(units - SLAB2_UNITS) * RATE3_PAISE;
        paise += part;
#ifdef DEBUG
        printf("[DEBUG]   slab 3: %d units x %ld = %ld paise\n", units - SLAB2_UNITS, RATE3_PAISE, part);
#endif
    }
    if (units > SLAB1_UNITS) {
        int upper = units < SLAB2_UNITS ? units : SLAB2_UNITS;
        long part = (long)(upper - SLAB1_UNITS) * RATE2_PAISE;
        paise += part;
#ifdef DEBUG
        printf("[DEBUG]   slab 2: %d units x %ld = %ld paise\n", upper - SLAB1_UNITS, RATE2_PAISE, part);
#endif
    }
    paise += (long)(units < SLAB1_UNITS ? units : SLAB1_UNITS) * RATE1_PAISE;
#ifdef DEBUG
    printf("[DEBUG]   total = %ld paise\n", paise);
#endif
    return paise;
}
