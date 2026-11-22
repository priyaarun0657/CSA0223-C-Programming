/*
 * Task 11 - the recommended design in use: fixed-width types from final_design.h,
 * a roll-over safe units calculation, a bill in paise_t, and a district-scale total
 * (800,000 bills) printed with the portable PRId64 format.
 */
#include <stdio.h>
#include <inttypes.h>
#include "final_design.h"

static units_t consumption(meter_t previous, meter_t current)
{
    return (meter_t)(current - previous);
}

static paise_t compute_bill(units_t units)
{
    paise_t paise = FIXED_CHARGE_PAISE;
    if (units > SLAB2_UNITS) paise += (paise_t)(units - SLAB2_UNITS) * RATE3_PAISE;
    if (units > SLAB1_UNITS) {
        units_t upper = units < SLAB2_UNITS ? units : SLAB2_UNITS;
        paise += (paise_t)(upper - SLAB1_UNITS) * RATE2_PAISE;
    }
    paise += (paise_t)(units < SLAB1_UNITS ? units : SLAB1_UNITS) * RATE1_PAISE;
    return paise;
}

int main(void)
{
    printf("Tariff in force: fixed %" PRId64 ", slab 2 rate %" PRId64 " paise/unit\n",
           (int64_t)FIXED_CHARGE_PAISE, (int64_t)RATE2_PAISE);
    units_t tests[] = {0, 100, 150, 200, 350};
    for (int i = 0; i < 5; i++) {
        paise_t p = compute_bill(tests[i]);
        printf("%3u units -> Rs %" PRId64 ".%02" PRId64 "\n", (unsigned)tests[i], p / 100, p % 100);
    }
    printf("roll-over 65400 -> 120 : %u units\n", (unsigned)consumption(65400, 120));

    /* district scale: same pseudo-random bills as Task 10 */
    uint32_t state = 12345u;
    paise_t total = 0;
    for (int i = 0; i < 800000; i++) {
        state = state * 1664525u + 1013904223u;
        total += compute_bill((units_t)((state >> 16) % 401u));
    }
    printf("district total (800,000 bills) = %" PRId64 " paise = Rs %" PRId64 ".%02" PRId64 "\n",
           (int64_t)total, (int64_t)(total / 100), (int64_t)(total % 100));
    printf("sizeof(paise_t) = %zu bytes, sizeof(meter_t) = %zu bytes\n", sizeof(paise_t), sizeof(meter_t));
    return 0;
}
