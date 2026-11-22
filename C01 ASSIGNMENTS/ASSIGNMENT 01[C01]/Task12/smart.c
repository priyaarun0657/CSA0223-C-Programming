#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "smart.h"

/* ------------------------------------------------------------------ tariff file */
int load_tariff(const char *path, tariff_t *t, char *err, size_t err_size)
{
    struct { const char *key; int64_t *dst; int seen; } map[] = {
        {"FIXED_CHARGE_PAISE",      &t->fixed_paise,     0},
        {"SLAB1_KWH",               &t->slab1_kwh,       0},
        {"SLAB2_KWH",               &t->slab2_kwh,       0},
        {"RATE1_PAISE_PER_KWH",     &t->rate1,           0},
        {"RATE2_PAISE_PER_KWH",     &t->rate2,           0},
        {"RATE3_PAISE_PER_KWH",     &t->rate3,           0},
        {"PEAK_START_HOUR",         &t->peak_start_hour, 0},
        {"PEAK_END_HOUR",           &t->peak_end_hour,   0},
        {"PEAK_SURCHARGE_PERCENT",  &t->peak_percent,    0},
    };
    const size_t n = sizeof map / sizeof map[0];

    FILE *f = fopen(path, "r");
    if (!f) { snprintf(err, err_size, "cannot open tariff file %s", path); return -1; }

    char line[128], key[48];
    long long value;
    int lineno = 0;
    while (fgets(line, sizeof line, f)) {
        lineno++;
        const char *p = line;
        while (isspace((unsigned char)*p)) p++;
        if (*p == '#' || *p == '\0') continue;
        if (sscanf(p, "%47[A-Z0-9_] = %lld", key, &value) != 2) {
            snprintf(err, err_size, "tariff file line %d is not KEY = number", lineno);
            fclose(f); return -1;
        }
        size_t i;
        for (i = 0; i < n; i++)
            if (strcmp(key, map[i].key) == 0) break;
        if (i == n) { snprintf(err, err_size, "tariff file line %d: unknown key %s", lineno, key); fclose(f); return -1; }
        if (value < 0) { snprintf(err, err_size, "tariff file line %d: %s must not be negative", lineno, key); fclose(f); return -1; }
        *map[i].dst = value;
        map[i].seen = 1;
    }
    fclose(f);

    for (size_t i = 0; i < n; i++)
        if (!map[i].seen) { snprintf(err, err_size, "tariff file is missing %s", map[i].key); return -1; }
    if (t->slab1_kwh >= t->slab2_kwh)                 { snprintf(err, err_size, "SLAB1_KWH must be below SLAB2_KWH"); return -1; }
    if (t->peak_start_hour >= t->peak_end_hour || t->peak_end_hour > 24)
                                                      { snprintf(err, err_size, "peak window must satisfy start < end <= 24"); return -1; }
    if (t->rate1 > MAX_RATE_PAISE_KWH || t->rate2 > MAX_RATE_PAISE_KWH || t->rate3 > MAX_RATE_PAISE_KWH)
                                                      { snprintf(err, err_size, "a rate is above %lld paise per kWh", MAX_RATE_PAISE_KWH); return -1; }
    if (t->peak_percent > MAX_PEAK_PERCENT)           { snprintf(err, err_size, "peak surcharge above %lld percent", MAX_PEAK_PERCENT); return -1; }
    return 0;
}

/* ------------------------------------------------------------------ readings */
int is_peak_slot(const tariff_t *t, unsigned slot_in_day)
{
    unsigned hour = slot_in_day / SLOTS_PER_HOUR;          /* slot 72 = 18:00, slot 87 = 21:45 */
    return (int64_t)hour >= t->peak_start_hour && (int64_t)hour < t->peak_end_hour;
}

void usage_add(const tariff_t *t, usage_t *u, uint32_t reading_index, wh_t wh)
{
    u->total_wh += wh;
    if (is_peak_slot(t, reading_index % SLOTS_PER_DAY)) u->peak_wh += wh;
    u->readings++;
}

/* One reading (Wh per 15 minutes) per line; the first line is 00:00-00:15 of day 1. */
int read_usage(const char *path, const tariff_t *t, usage_t *u, char *err, size_t err_size)
{
    FILE *f = fopen(path, "r");
    if (!f) { snprintf(err, err_size, "cannot open readings file %s", path); return -1; }
    memset(u, 0, sizeof *u);

    char line[64];
    uint32_t index = 0;
    int lineno = 0;
    while (fgets(line, sizeof line, f)) {
        lineno++;
        const char *p = line;
        while (isspace((unsigned char)*p)) p++;
        if (*p == '\0') continue;
        if (*p == '-') { snprintf(err, err_size, "line %d: negative reading", lineno); fclose(f); return -1; }
        char *end;
        errno = 0;
        unsigned long long v = strtoull(p, &end, 10);
        while (isspace((unsigned char)*end)) end++;
        if (end == p || *end != '\0' || errno == ERANGE) {
            snprintf(err, err_size, "line %d: not a whole number of Wh", lineno); fclose(f); return -1;
        }
        if (v > MAX_INTERVAL_WH) {
            snprintf(err, err_size, "line %d: %llu Wh in 15 minutes exceeds the %llu Wh limit", lineno, v, MAX_INTERVAL_WH);
            fclose(f); return -1;
        }
        if (u->readings >= MAX_READINGS_PER_BILL) {
            snprintf(err, err_size, "more than %llu readings: a bill covers at most 31 days", MAX_READINGS_PER_BILL);
            fclose(f); return -1;
        }
        usage_add(t, u, index++, (wh_t)v);
    }
    fclose(f);
    if (u->readings == 0) { snprintf(err, err_size, "readings file is empty"); return -1; }
    return 0;
}

/* ------------------------------------------------------------------ the bill
 * Slabs are applied to the month's TOTAL energy in Wh. Every term is kept in
 * "milli-paise" (paise x 1000) so that nothing is rounded until the very end:
 *     energy_milli = sum( Wh_in_slab * rate_paise_per_kWh )
 * Peak surcharge = peak_percent% of the part of the energy charge that belongs to peak Wh
 * (average-rate allocation):
 *     surcharge_milli = energy_milli * peak_wh * pct / (total_wh * 100)
 * To keep every intermediate product inside 64 bits the division is split into quotient and
 * remainder (bounds worked out in REPORT: largest product 2.2e16). The energy total is then
 * rounded ONCE, half-up, to whole paise.
 */
int compute_bill_wh(const tariff_t *t, uint64_t total_wh, uint64_t peak_wh, paise_t *bill)
{
    if (peak_wh > total_wh)      return 1;
    if (total_wh > MAX_TOTAL_WH) return 2;

    const uint64_t s1 = (uint64_t)t->slab1_kwh * WH_PER_KWH;
    const uint64_t s2 = (uint64_t)t->slab2_kwh * WH_PER_KWH;

    uint64_t tier1 = total_wh < s1 ? total_wh : s1;
    uint64_t tier2 = total_wh > s1 ? (total_wh < s2 ? total_wh : s2) - s1 : 0;
    uint64_t tier3 = total_wh > s2 ? total_wh - s2 : 0;

    uint64_t energy_milli = tier1 * (uint64_t)t->rate1 + tier2 * (uint64_t)t->rate2 + tier3 * (uint64_t)t->rate3;

    uint64_t surcharge_milli = 0;
    if (total_wh > 0 && peak_wh > 0 && t->peak_percent > 0) {
        uint64_t q = energy_milli / total_wh;
        uint64_t r = energy_milli % total_wh;
        uint64_t pct = (uint64_t)t->peak_percent;
        /* value x 100 = q*peak*pct + r*peak*pct/total_wh */
        uint64_t x100 = q * peak_wh * pct + (r * peak_wh * pct) / total_wh;
        surcharge_milli = x100 / 100;
    }

    uint64_t total_milli = energy_milli + surcharge_milli;
    *bill = t->fixed_paise + (paise_t)((total_milli + 500) / 1000);     /* one rounding, half-up */
    return 0;
}
