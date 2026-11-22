/*
 * billing.h - the complete billing program of Grama Jyothi Rural Electricity Co-operative.
 * ALL tariff values live here and nowhere else: a tariff revision is a change to this file only.
 */
#ifndef BILLING_H
#define BILLING_H

/* ---- tariff (money in PAISE, so there is never a rounding error) ---- */
#define FIXED_CHARGE_PAISE  5000L     /* Rs 50.00 fixed charge           */
#define SLAB1_UNITS         100       /* units 1..100 : free             */
#define SLAB2_UNITS         200       /* units 101..200                  */
#define RATE1_PAISE         0L        /* Rs 0.00 per unit                */
#define RATE2_PAISE         225L      /* Rs 2.25 per unit                */
#define RATE3_PAISE         450L      /* Rs 4.50 per unit above 200      */

/* ---- meter and validation limits ---- */
#define MAX_READING         65535L    /* 16-bit register                 */
#define MAX_MONTHLY_UNITS   5000U     /* more than this is a fault/typing error, not a household */

typedef unsigned short meter_t;       /* 16-bit meter register */

typedef enum {
    READ_OK = 0,
    READ_NEGATIVE,                    /* a reading below 0               */
    READ_OUT_OF_RANGE,                /* a reading above 65535           */
    READ_IMPLAUSIBLE                  /* consumption above MAX_MONTHLY_UNITS (e.g. current < previous with no real roll-over) */
} read_status_t;

/* Units used between two readings, roll-over safe. Returns READ_OK or the reason it was rejected. */
read_status_t compute_units(long previous, long current, unsigned int *units);

/* Bill in paise for the given units. */
long compute_bill(unsigned int units);

const char *status_text(read_status_t status);

#endif /* BILLING_H */
