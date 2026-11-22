/*
 * Task 02 - Alternative concept: money stored as double RUPEES instead of
 * long PAISE. Shows where rounding must be applied (once, when converting the
 * double total back to paise) and how the error grows with the number of bills.
 */
#include <stdio.h>
#include <math.h>
#include <time.h>

int main(void)
{
    int counts[] = {4000, 60000, 800000};
    printf("Each bill = Rs 123.45 (12345 paise)\n");
    printf("%8s  %-22s %-16s %-18s\n", "n", "double total (rupees)", "llround(x*100)", "exact paise");
    for (int k = 0; k < 3; k++) {
        int n = counts[k];
        double d_total = 0.0;
        long long p_total = 0;
        for (int i = 0; i < n; i++) { d_total += 123.45; p_total += 12345; }
        long long rounded = llround(d_total * 100.0);
        printf("%8d  %-22.10f %-16lld %-18lld error before rounding: %.3e rupees\n",
               n, d_total, rounded, p_total, d_total - (double)p_total / 100.0);
    }

    /* time per addition and space per value */
    const long reps = 100000000L;
    volatile double dv = 0.0;
    volatile long long lv = 0;
    clock_t t0 = clock();
    for (long i = 0; i < reps; i++) dv += 0.01;
    clock_t t1 = clock();
    for (long i = 0; i < reps; i++) lv += 1;
    clock_t t2 = clock();
    printf("\ntime per addition: double %.3f ns, long long %.3f ns (both O(1))\n",
           1e9 * (double)(t1 - t0) / CLOCKS_PER_SEC / reps,
           1e9 * (double)(t2 - t1) / CLOCKS_PER_SEC / reps);
    printf("space per value  : double %zu bytes, long long %zu bytes\n", sizeof(double), sizeof(long long));
    printf("note: 100,000,000 additions of 0.01 in double give %.6f (exact 1000000.000000)\n", (double)dv);
    return 0;
}
