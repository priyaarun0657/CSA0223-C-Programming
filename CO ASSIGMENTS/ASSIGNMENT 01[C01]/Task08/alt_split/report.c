#include <stdio.h>
#include "billing8.h"

void print_report(const int *units, int n)
{
    for (int i = 0; i < n; i++) {
        long p = compute_bill(units[i]);
        printf("%3d units -> Rs %ld.%02ld\n", units[i], p / 100, p % 100);
    }
}

int main(void)
{
    int units[16];
    int n = read_units(units, 16);
    print_report(units, n);
    return 0;
}
