/*
 * Task 06 - Read and print data correctly with scanf / printf.
 * Input lines : household category previous current      e.g.  101 D 1200 1530
 * Every scanf return value is checked, so letters or missing fields are
 * detected, the bad line is discarded and the program carries on.
 * Money is held in paise (long) and printed as Rs <paise/100>.<paise%100, 2 digits>.
 */
#include <stdio.h>

#define FIXED_CHARGE_PAISE 5000L
#define SLAB1_UNITS        100
#define SLAB2_UNITS        200
#define RATE1_PAISE        0L
#define RATE2_PAISE        225L
#define RATE3_PAISE        450L
#define MAX_READING        65535L
#define MAX_MONTHLY_UNITS  5000L

typedef unsigned short meter_t;

static long compute_bill(unsigned int units)
{
    long paise = FIXED_CHARGE_PAISE;
    if (units > SLAB2_UNITS) paise += (long)(units - SLAB2_UNITS) * RATE3_PAISE;
    if (units > SLAB1_UNITS) {
        unsigned int upper = units < SLAB2_UNITS ? units : SLAB2_UNITS;
        paise += (long)(upper - SLAB1_UNITS) * RATE2_PAISE;
    }
    paise += (long)(units < SLAB1_UNITS ? units : SLAB1_UNITS) * RATE1_PAISE;
    return paise;
}

int main(void)
{
    long household, previous, current;      /* %ld */
    char category;                          /* %c  */
    int  fields;                            /* return value of scanf */
    int  record = 0, valid = 0, rejected = 0;
    long long district_total = 0;           /* %lld */

    printf("Enter: household category(D/C) previous current   (end with Ctrl+D, or Ctrl+Z on Windows)\n");
    while ((fields = scanf("%ld %c %ld %ld", &household, &category, &previous, &current)) != EOF) {
        record++;
        if (fields != 4) {                                   /* letters, missing numbers ... */
            printf("Record %d: INVALID INPUT - skipped\n", record);
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { }   /* discard the rest of the line */
            rejected++;
            continue;
        }
        if (category != 'D' && category != 'C') {
            printf("Record %d: INVALID CATEGORY '%c' - skipped\n", record, category);
            rejected++;
            continue;
        }
        if (previous < 0 || current < 0) {
            printf("Record %d: NEGATIVE READING - skipped\n", record);
            rejected++;
            continue;
        }
        if (previous > MAX_READING || current > MAX_READING) {
            printf("Record %d: READING ABOVE 65535 - skipped\n", record);
            rejected++;
            continue;
        }
        unsigned int units = (meter_t)((meter_t)current - (meter_t)previous);   /* roll-over safe */
        if (units > MAX_MONTHLY_UNITS) {
            printf("Record %d: IMPLAUSIBLE CONSUMPTION (%u units) - skipped\n", record, units);
            rejected++;
            continue;
        }
        long bill = compute_bill(units);
        printf("Record %d: household %ld [%c] previous=%ld current=%ld units=%u bill=Rs %ld.%02ld\n",
               record, household, category, previous, current, units, bill / 100, bill % 100);
        district_total += bill;
        valid++;
    }
    printf("Summary: %d valid, %d rejected, total Rs %lld.%02lld\n",
           valid, rejected, district_total / 100, district_total % 100);
    return 0;
}
