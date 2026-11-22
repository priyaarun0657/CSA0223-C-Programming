#include "billing.h"

read_status_t compute_units(long previous, long current, unsigned int *units)
{
    if (previous < 0 || current < 0)
        return READ_NEGATIVE;
    if (previous > MAX_READING || current > MAX_READING)
        return READ_OUT_OF_RANGE;

    /* 16-bit unsigned arithmetic: a roll-over (65400 -> 120) gives 256, not a negative number */
    unsigned int used = (meter_t)((meter_t)current - (meter_t)previous);

    /* A 'decrease' without a real roll-over (e.g. 1530 -> 1200) also lands here as ~65206 units. */
    if (used > MAX_MONTHLY_UNITS)
        return READ_IMPLAUSIBLE;

    *units = used;
    return READ_OK;
}

long compute_bill(unsigned int units)
{
    long paise = FIXED_CHARGE_PAISE;
    if (units > SLAB2_UNITS)
        paise += (long)(units - SLAB2_UNITS) * RATE3_PAISE;
    if (units > SLAB1_UNITS) {
        unsigned int upper = units < SLAB2_UNITS ? units : SLAB2_UNITS;
        paise += (long)(upper - SLAB1_UNITS) * RATE2_PAISE;
    }
    paise += (long)(units < SLAB1_UNITS ? units : SLAB1_UNITS) * RATE1_PAISE;
    return paise;
}

const char *status_text(read_status_t status)
{
    switch (status) {
    case READ_OK:           return "OK";
    case READ_NEGATIVE:     return "NEGATIVE READING";
    case READ_OUT_OF_RANGE: return "READING ABOVE 65535";
    case READ_IMPLAUSIBLE:  return "IMPLAUSIBLE CONSUMPTION (decrease without a real roll-over, or meter fault)";
    }
    return "UNKNOWN";
}
