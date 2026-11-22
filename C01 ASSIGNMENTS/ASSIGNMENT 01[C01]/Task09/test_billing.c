/*
 * Table-driven tests for compute_units() and compute_bill().
 * Every row has an EXPECTED result written by hand before running; the program
 * prints expected and actual side by side and returns non-zero if any row fails.
 */
#include <stdio.h>
#include "billing.h"

struct test_case {
    const char   *name;
    long          previous, current;
    read_status_t expected_status;
    unsigned int  expected_units;    /* meaningful when expected_status == READ_OK */
    long          expected_bill;     /* paise; meaningful when expected_status == READ_OK */
};

static const struct test_case cases[] = {
    /* normal bills in each slab */
    {"slab 1 normal (50 units)",        100,    150, READ_OK,           50,   5000L},
    {"slab 2 normal (150 units)",       3000,  3150, READ_OK,          150,  16250L},
    {"slab 3 normal (350 units)",       10000,10350, READ_OK,          350,  95000L},
    /* boundaries */
    {"zero units",                      500,    500, READ_OK,            0,   5000L},
    {"exactly 100 units",               1000,  1100, READ_OK,          100,   5000L},
    {"101 units",                       1000,  1101, READ_OK,          101,   5225L},
    {"exactly 200 units",               0,      200, READ_OK,          200,  27500L},
    {"201 units",                       0,      201, READ_OK,          201,  27950L},
    {"maximum plausible 5000 units",    0,     5000, READ_OK,         5000, 2187500L},
    /* roll-over */
    {"roll-over 65535 -> 0 (1 unit)",   65535,    0, READ_OK,            1,   5000L},
    {"roll-over 65400 -> 120",          65400,  120, READ_OK,          256,  52700L},
    {"roll-over 65535 -> 99 (100)",     65535,   99, READ_OK,          100,   5000L},
    /* invalid input */
    {"negative previous reading",       -5,     100, READ_NEGATIVE,      0,      0},
    {"negative current reading",        100,    -1,  READ_NEGATIVE,      0,      0},
    {"reading above 65535",             70000,   10, READ_OUT_OF_RANGE,  0,      0},
    {"current < previous, no roll-over",1530,  1200, READ_IMPLAUSIBLE,   0,      0},
    {"5001 units (above the limit)",    0,     5001, READ_IMPLAUSIBLE,   0,      0},
};

int main(void)
{
    int failed = 0;
    int n = (int)(sizeof cases / sizeof cases[0]);
    printf("%-34s %-22s %-14s %-14s %s\n", "CASE", "readings", "EXPECTED", "ACTUAL", "RESULT");
    for (int i = 0; i < n; i++) {
        const struct test_case *c = &cases[i];
        unsigned int units = 0;
        read_status_t st = compute_units(c->previous, c->current, &units);
        long bill = (st == READ_OK) ? compute_bill(units) : 0;

        char readings[40], expected[40], actual[40];
        snprintf(readings, sizeof readings, "%ld -> %ld", c->previous, c->current);
        if (c->expected_status == READ_OK)
            snprintf(expected, sizeof expected, "%u u, Rs %ld.%02ld", c->expected_units, c->expected_bill / 100, c->expected_bill % 100);
        else
            snprintf(expected, sizeof expected, "reject (%d)", (int)c->expected_status);
        if (st == READ_OK)
            snprintf(actual, sizeof actual, "%u u, Rs %ld.%02ld", units, bill / 100, bill % 100);
        else
            snprintf(actual, sizeof actual, "reject (%d)", (int)st);

        int pass = (st == c->expected_status) &&
                   (st != READ_OK || (units == c->expected_units && bill == c->expected_bill));
        if (!pass) failed++;
        printf("%-34s %-22s %-14s %-14s %s\n", c->name, readings, expected, actual, pass ? "PASS" : "FAIL");
    }

    /* five bills whose total was calculated by hand:
     *   0 u -> 50.00, 100 u -> 50.00, 150 u -> 162.50, 200 u -> 275.00, 350 u -> 950.00
     *   50.00 + 50.00 + 162.50 + 275.00 + 950.00 = Rs 1487.50 = 148750 paise */
    unsigned int five[] = {0, 100, 150, 200, 350};
    long sum = 0;
    for (int i = 0; i < 5; i++) sum += compute_bill(five[i]);
    int pass = (sum == 148750L);
    if (!pass) failed++;
    printf("%-34s %-22s %-14s %-14s %s\n", "hand-calculated total of 5 bills", "0,100,150,200,350",
           "Rs 1487.50", "", pass ? "PASS" : "FAIL");
    printf("  (actual total printed below)\n  total = Rs %ld.%02ld\n", sum / 100, sum % 100);

    printf("\n%d of %d checks failed\n", failed, n + 1);
    return failed != 0;
}
