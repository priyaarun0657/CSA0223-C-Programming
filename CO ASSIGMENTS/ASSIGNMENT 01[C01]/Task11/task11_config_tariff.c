/*
 * Task 11 - Alternative concept: the tariff is READ FROM A CONFIGURATION FILE when the
 * program starts instead of being compiled in. A revision = edit the text file; no recompile.
 * Usage: ./task11_config_tariff.out tariff.cfg
 * Also measures the start-up cost of reading the file.
 */
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
    long fixed_paise;
    long slab1_units, slab2_units;
    long rate1_paise, rate2_paise, rate3_paise;
} tariff_t;

static int load_tariff(const char *path, tariff_t *t)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    char line[128], key[48];
    long value;
    unsigned seen = 0;
    while (fgets(line, sizeof line, f)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        if (sscanf(line, " %47[A-Z0-9_] = %ld", key, &value) != 2) { fclose(f); return -2; }
        if      (!strcmp(key, "FIXED_CHARGE_PAISE")) { t->fixed_paise = value; seen |= 1u;  }
        else if (!strcmp(key, "SLAB1_UNITS"))        { t->slab1_units = value; seen |= 2u;  }
        else if (!strcmp(key, "SLAB2_UNITS"))        { t->slab2_units = value; seen |= 4u;  }
        else if (!strcmp(key, "RATE1_PAISE"))        { t->rate1_paise = value; seen |= 8u;  }
        else if (!strcmp(key, "RATE2_PAISE"))        { t->rate2_paise = value; seen |= 16u; }
        else if (!strcmp(key, "RATE3_PAISE"))        { t->rate3_paise = value; seen |= 32u; }
        else { fclose(f); return -3; }                         /* unknown key */
    }
    fclose(f);
    if (seen != 63u) return -4;                                /* a value is missing */
    if (t->slab1_units >= t->slab2_units || t->fixed_paise < 0 ||
        t->rate1_paise < 0 || t->rate2_paise < 0 || t->rate3_paise < 0) return -5;   /* nonsense values */
    return 0;
}

static long compute_bill(const tariff_t *t, long units)
{
    long paise = t->fixed_paise;
    if (units > t->slab2_units) paise += (units - t->slab2_units) * t->rate3_paise;
    if (units > t->slab1_units) {
        long upper = units < t->slab2_units ? units : t->slab2_units;
        paise += (upper - t->slab1_units) * t->rate2_paise;
    }
    paise += (units < t->slab1_units ? units : t->slab1_units) * t->rate1_paise;
    return paise;
}

int main(int argc, char *argv[])
{
    const char *path = argc > 1 ? argv[1] : "tariff.cfg";
    tariff_t t;
    int rc = load_tariff(path, &t);
    if (rc != 0) { printf("cannot use %s (error %d)\n", path, rc); return 1; }

    printf("tariff loaded from %s: fixed %ld, slabs %ld/%ld, rates %ld/%ld/%ld paise\n", path,
           t.fixed_paise, t.slab1_units, t.slab2_units, t.rate1_paise, t.rate2_paise, t.rate3_paise);
    long tests[] = {0, 100, 150, 200, 350};
    for (int i = 0; i < 5; i++) {
        long p = compute_bill(&t, tests[i]);
        printf("%3ld units -> Rs %ld.%02ld\n", tests[i], p / 100, p % 100);
    }

    const int reps = 20000;
    clock_t t0 = clock();
    for (int i = 0; i < reps; i++) load_tariff(path, &t);
    clock_t t1 = clock();
    printf("start-up cost: %.2f microseconds to read and validate the file (average of %d reads)\n",
           1e6 * (double)(t1 - t0) / CLOCKS_PER_SEC / reps, reps);
    printf("space for the six tariff values: %zu bytes (a #define occupies none)\n", sizeof(tariff_t));
    return 0;
}
