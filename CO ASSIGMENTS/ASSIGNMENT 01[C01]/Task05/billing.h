/*
 * billing.h - tariff constants and function prototypes of the billing program.
 *
 * Include guard: if two .c files (or one file twice) include this header, the
 * second inclusion is skipped, so nothing is defined twice.
 *
 * #include <stdio.h>  -> angle brackets: search the compiler's system directories
 * #include "billing.h" -> quotes: search the current directory first, then system
 */
#ifndef BILLING_H
#define BILLING_H

#define SLAB1_UNITS        100        /* first 100 units            */
#define SLAB2_UNITS        200        /* units 101 to 200           */
#define RATE1_PAISE        0L         /* first slab is free         */
#define RATE2_PAISE        225L       /* Rs 2.25 per unit           */
#define RATE3_PAISE        450L       /* Rs 4.50 per unit above 200 */
#define FIXED_CHARGE_PAISE 5000L      /* Rs 50 fixed charge         */

long compute_bill(int units);         /* returns the bill in paise  */

#endif /* BILLING_H */
