#ifndef BILLING8_H
#define BILLING8_H

#define FIXED_CHARGE_PAISE 5000L
#define SLAB1_UNITS        100
#define SLAB2_UNITS        200
#define RATE2_PAISE        225L
#define RATE3_PAISE        450L

int  read_units(int *units, int max);          /* input.c  */
long compute_bill(int units);                  /* tariff.c */
void print_report(const int *units, int n);   /* report.c */

#endif
