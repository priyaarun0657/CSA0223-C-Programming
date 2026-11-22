/*
 * Task 06 - Alternative concept: read each line with fgets() and convert with
 * strtol() instead of scanf(). One bad line can never leave the input stream in
 * an unknown state, because the whole line is already in the buffer.
 * Output format is identical to task06_billing_io.c so the two can be diff'ed.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define FIXED_CHARGE_PAISE 5000L
#define SLAB1_UNITS        100
#define SLAB2_UNITS        200
#define RATE1_PAISE        0L
#define RATE2_PAISE        225L
#define RATE3_PAISE        450L
#define MAX_READING        65535L
#define MAX_MONTHLY_UNITS  5000L
#define LINE_MAX_CHARS     80            /* buffer space: one line of up to 79 characters */

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

static long next_long(const char **p, int *ok)
{
    char *end;
    errno = 0;
    long v = strtol(*p, &end, 10);
    if (end == *p || errno == ERANGE) *ok = 0;      /* no digits, or too large */
    *p = end;
    return v;
}

/* returns 1 when the line is "<long> <char> <long> <long>" and nothing else */
static int parse_record(const char *line, long *hh, char *cat, long *prev, long *curr)
{
    const char *p = line;
    int ok = 1;
    *hh = next_long(&p, &ok);
    if (!ok) return 0;
    while (isspace((unsigned char)*p)) p++;
    if (*p == '\0') return 0;
    *cat = *p++;
    *prev = next_long(&p, &ok);
    *curr = next_long(&p, &ok);
    if (!ok) return 0;
    while (isspace((unsigned char)*p)) p++;
    return *p == '\0';                               /* reject trailing junk */
}

int main(void)
{
    char line[LINE_MAX_CHARS];
    long household, previous, current;
    char category;
    int record = 0, valid = 0, rejected = 0;
    long long district_total = 0;

    printf("Enter: household category(D/C) previous current   (end with Ctrl+D, or Ctrl+Z on Windows)\n");
    while (fgets(line, sizeof line, stdin) != NULL) {
        if (strchr(line, '\n') == NULL && !feof(stdin)) {      /* line longer than the buffer */
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { }
        }
        const char *q = line;
        while (isspace((unsigned char)*q)) q++;
        if (*q == '\0') continue;                              /* blank line: not a record */
        record++;
        if (!parse_record(line, &household, &category, &previous, &current)) {
            printf("Record %d: INVALID INPUT - skipped\n", record);
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
        unsigned int units = (meter_t)((meter_t)current - (meter_t)previous);
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
