/*
 * Task 01 - Understand the problem and identify the data
 * Grama Jyothi Rural Electricity Co-operative
 *
 * Prints the data dictionary of the billing problem (what is stored, why,
 * which type) and then checks the "rules every correct bill must satisfy"
 * on sample bills and on every consumption from 0 to 5000 units.
 */
#include <stdio.h>

#define FIXED_CHARGE_PAISE 5000L
#define SLAB1_UNITS        100
#define SLAB2_UNITS        200
#define RATE1_PAISE        0L
#define RATE2_PAISE        225L
#define RATE3_PAISE        450L
#define MAX_MONTHLY_UNITS  5000

typedef unsigned short meter_t;           /* 16-bit meter register */

struct item {
    const char *name;
    const char *role;                      /* input / output / constant */
    const char *type;
    const char *justification;
};

static const struct item items[] = {
    {"household number", "input",    "unsigned long", "800,000 houses > 65535, never negative"},
    {"category",         "input",    "char",          "one letter code: 'D' domestic, 'C' commercial"},
    {"previous reading", "input",    "meter_t (u16)", "meter register is 16-bit: 0..65535"},
    {"current reading",  "input",    "meter_t (u16)", "same register; may be LOWER after roll-over"},
    {"units",            "output",   "unsigned int",  "(current-previous) mod 65536; never negative"},
    {"fixed charge",     "constant", "long (paise)",   "Rs 50.00 = 5000 paise, integer so no rounding"},
    {"slab limits",      "constant", "int (units)",    "100 and 200 units, whole units"},
    {"slab rates",       "constant", "long (paise)",   "0, 225, 450 paise per unit (fractional rupees!)"},
    {"energy charge",    "output",   "long (paise)",   "up to 5000*450 = 2,250,000 > 32767"},
    {"total bill",       "output",   "long (paise)",   "fixed + energy; never negative"},
    {"district total",   "output",   "long long",      "800,000 bills * ~Rs 415 = 3.3e10 paise > INT_MAX"},
};

static long energy_charge(unsigned int units)
{
    long e = 0;
    if (units > SLAB2_UNITS) e += (long)(units - SLAB2_UNITS) * RATE3_PAISE;
    if (units > SLAB1_UNITS) {
        unsigned int upper = units < SLAB2_UNITS ? units : SLAB2_UNITS;
        e += (long)(upper - SLAB1_UNITS) * RATE2_PAISE;
    }
    e += (long)(units < SLAB1_UNITS ? units : SLAB1_UNITS) * RATE1_PAISE;
    return e;
}

int main(void)
{
    printf("%-18s %-9s %-15s %s\n", "DATA ITEM", "ROLE", "TYPE", "JUSTIFICATION");
    for (size_t i = 0; i < sizeof items / sizeof items[0]; i++)
        printf("%-18s %-9s %-15s %s\n", items[i].name, items[i].role,
               items[i].type, items[i].justification);

    printf("\nRule checks on every consumption 0..%d units:\n", MAX_MONTHLY_UNITS);
    int checks = 0, failed = 0;
    long previous_total = -1;
    for (unsigned int u = 0; u <= MAX_MONTHLY_UNITS; u++) {
        long energy = energy_charge(u);
        long total  = FIXED_CHARGE_PAISE + energy;
        checks += 3;
        if (total < 0)                           { failed++; printf("FAIL negative bill at %u\n", u); }
        if (total != FIXED_CHARGE_PAISE + energy) { failed++; printf("FAIL parts != total at %u\n", u); }
        if (total < previous_total)              { failed++; printf("FAIL bill decreased at %u\n", u); }
        previous_total = total;
    }
    printf("  rule 1: bill never negative\n");
    printf("  rule 2: total == fixed + energy\n");
    printf("  rule 3: more units never gives a smaller bill\n");
    printf("  %d checks run, %d failed\n", checks, failed);

    printf("\nSample bills (units -> fixed + energy = total):\n");
    unsigned int sample[] = {0, 100, 150, 200, 350};
    for (size_t i = 0; i < sizeof sample / sizeof sample[0]; i++) {
        long e = energy_charge(sample[i]);
        long t = FIXED_CHARGE_PAISE + e;
        printf("  %3u units -> Rs %ld.%02ld + Rs %ld.%02ld = Rs %ld.%02ld\n", sample[i],
               FIXED_CHARGE_PAISE / 100, FIXED_CHARGE_PAISE % 100,
               e / 100, e % 100, t / 100, t % 100);
    }
    printf("\nsizeof(meter_t) = %zu bytes\n", sizeof(meter_t));
    return failed != 0;
}
