/*
 * final_design.h - the RECOMMENDED data types, constants and preprocessor features
 * (justified in RECOMMENDATION.md and by the measurements of Tasks 2, 7, 9 and 10).
 */
#ifndef FINAL_DESIGN_H
#define FINAL_DESIGN_H

#include <stdint.h>

/* ---- types: fixed-width, so the size is the same on every compiler ---- */
typedef uint16_t meter_t;      /* 16-bit meter register, wraps at 65536              */
typedef uint32_t units_t;      /* consumption in whole units                          */
typedef int64_t  paise_t;      /* EVERY money value, bill or district total, in paise */

/* ---- tariff: one place, one line per value (the only thing edited in a revision) ---- */
#define FIXED_CHARGE_PAISE  ((paise_t)5000)
#define SLAB1_UNITS         ((units_t)100)
#define SLAB2_UNITS         ((units_t)200)
#define RATE1_PAISE         ((paise_t)0)
#define RATE2_PAISE         ((paise_t)225)
#define RATE3_PAISE         ((paise_t)450)

/* ---- validation limits ---- */
#define MAX_READING         65535L
#define MAX_MONTHLY_UNITS   ((units_t)5000)

/* ---- compile-time guards: the build FAILS if a platform breaks an assumption ---- */
_Static_assert(sizeof(meter_t) * 8 == 16, "meter register must be 16 bits");
_Static_assert(sizeof(paise_t) >= 8,      "money must be at least 64 bits for district totals");
_Static_assert(SLAB1_UNITS < SLAB2_UNITS, "slab limits must increase");

/* ---- one safe macro, fully parenthesised ---- */
#define RUPEES_TO_PAISE(r)  ((paise_t)(r) * 100)

#endif /* FINAL_DESIGN_H */
