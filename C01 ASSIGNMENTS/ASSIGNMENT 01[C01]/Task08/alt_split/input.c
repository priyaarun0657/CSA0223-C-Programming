#include <stdio.h>
#include "billing8.h"

/* Stands in for reading the meter file: returns the number of consumptions stored. */
int read_units(int *units, int max)
{
    static const int sample[] = {0, 100, 150, 200, 350};
    int n = (int)(sizeof sample / sizeof sample[0]);
    if (n > max) n = max;
    for (int i = 0; i < n; i++) units[i] = sample[i];
    return n;
}
