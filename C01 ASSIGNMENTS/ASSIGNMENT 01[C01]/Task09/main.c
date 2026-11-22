/*
 * Billing program. Reads one record per line from standard input:
 *     household category previous current          e.g.   101 D 1200 1530
 * Lines are read with fgets() and converted with strtol(), so a bad line (letters,
 * missing or extra fields) is rejected as a whole and can never disturb the next line.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "billing.h"

#define LINE_CHARS 100

static long next_long(const char **p, int *ok)
{
    char *end;
    errno = 0;
    long v = strtol(*p, &end, 10);
    if (end == *p || errno == ERANGE) *ok = 0;
    *p = end;
    return v;
}

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
    return *p == '\0';
}

int main(void)
{
    char line[LINE_CHARS];
    long household, previous, current;
    char category;
    int record = 0, valid = 0, rejected = 0;
    long long total = 0;

    while (fgets(line, sizeof line, stdin) != NULL) {
        if (strchr(line, '\n') == NULL && !feof(stdin)) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) { }
        }
        const char *q = line;
        while (isspace((unsigned char)*q)) q++;
        if (*q == '\0') continue;
        record++;

        if (!parse_record(line, &household, &category, &previous, &current)) {
            printf("Record %d: REJECTED - not in the form: household category previous current\n", record);
            rejected++;
            continue;
        }
        if (category != 'D' && category != 'C') {
            printf("Record %d: REJECTED - category '%c' is not D or C\n", record, category);
            rejected++;
            continue;
        }
        unsigned int units = 0;
        read_status_t st = compute_units(previous, current, &units);
        if (st != READ_OK) {
            printf("Record %d: REJECTED - %s (household %ld, previous=%ld, current=%ld)\n",
                   record, status_text(st), household, previous, current);
            rejected++;
            continue;
        }
        long bill = compute_bill(units);
        printf("Record %d: household %ld [%c] previous=%ld current=%ld units=%u bill=Rs %ld.%02ld\n",
               record, household, category, previous, current, units, bill / 100, bill % 100);
        total += bill;
        valid++;
    }
    printf("Summary: %d bills, %d rejected, total collected Rs %lld.%02lld\n",
           valid, rejected, total / 100, total % 100);
    return 0;
}
