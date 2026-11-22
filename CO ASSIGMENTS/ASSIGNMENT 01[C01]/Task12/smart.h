/*
 * smart.h - Day-4 redesign: smart meters (15-minute watt-hour readings), a time-of-day
 * peak surcharge, and a tariff read from a file at run time.
 */
#ifndef SMART_H
#define SMART_H

#include <stddef.h>
#include <stdint.h>

/* ---- physical / structural constants: these never change, so #define stays ---- */
#define SLOTS_PER_HOUR        4
#define SLOTS_PER_DAY         96                 /* 24 h x 4 readings               */
#define WH_PER_KWH            1000ULL
#define MAX_INTERVAL_WH       5000ULL            /* 20 kW connection x 0.25 h       */
#define MAX_READINGS_PER_BILL 2976ULL            /* 31 days x 96 readings           */
#define MAX_TOTAL_WH          (MAX_INTERVAL_WH * MAX_READINGS_PER_BILL)   /* 14,880,000 Wh */
#define MAX_RATE_PAISE_KWH    2000LL             /* sanity limit: Rs 20 per unit    */
#define MAX_PEAK_PERCENT      100LL

/* ---- types ---- */
typedef uint32_t wh_t;          /* one reading in Wh (max 5000, but 32 bits leaves headroom)  */
typedef int64_t  paise_t;       /* all money                                                   */

/* ---- tariff: loaded from a text file, NOT compiled in ---- */
typedef struct {
    int64_t fixed_paise;
    int64_t slab1_kwh, slab2_kwh;              /* slab limits in whole kWh                */
    int64_t rate1, rate2, rate3;               /* paise PER kWh                           */
    int64_t peak_start_hour, peak_end_hour;    /* peak = start <= hour < end              */
    int64_t peak_percent;                      /* surcharge on peak energy, e.g. 20       */
} tariff_t;

/* ---- what one bill needs from the readings ---- */
typedef struct {
    uint64_t total_wh;
    uint64_t peak_wh;
    uint32_t readings;
} usage_t;

int  load_tariff(const char *path, tariff_t *t, char *err, size_t err_size);
int  is_peak_slot(const tariff_t *t, unsigned slot_in_day);
void usage_add(const tariff_t *t, usage_t *u, uint32_t reading_index, wh_t wh);
int  read_usage(const char *path, const tariff_t *t, usage_t *u, char *err, size_t err_size);

/* returns 0 = ok, 1 = peak_wh > total_wh, 2 = total_wh above MAX_TOTAL_WH */
int  compute_bill_wh(const tariff_t *t, uint64_t total_wh, uint64_t peak_wh, paise_t *bill);

#endif /* SMART_H */
