/*
 * Task 07 - Alternative concept: do all calculations in float RUPEES and round
 * only once at the end, instead of integer paise. Measures how the error of
 * the total grows with the number of bills n, and the time/space per value.
 */
#include <stdio.h>
#include <math.h>
#include <time.h>

static unsigned int state = 777u;
static unsigned int next_units(void)
{
    state = state * 1664525u + 1013904223u;
    return (state >> 16) % 401u;                 /* 0..400 units */
}

static long bill_paise(unsigned int u)
{
    long p = 5000L;
    if (u > 200) p += (long)(u - 200) * 450L;
    if (u > 100) p += (long)((u < 200 ? u : 200) - 100) * 225L;
    return p;
}

int main(void)
{
    int counts[] = {1000, 4000, 16000, 60000, 240000, 800000};
    printf("%8s %16s %16s %14s %14s\n", "n", "exact paise", "float paise", "error (paise)", "error/n");
    for (int k = 0; k < 6; k++) {
        int n = counts[k];
        state = 777u;
        long long exact = 0;
        float f_total = 0.0f;
        for (int i = 0; i < n; i++) {
            long p = bill_paise(next_units());
            exact += p;
            f_total += (float)p / 100.0f;         /* every bill held as float rupees */
        }
        long long f_paise = llroundf(f_total * 100.0f);     /* single rounding at the end */
        long long err = f_paise - exact;
        printf("%8d %16lld %16lld %14lld %14.5f\n", n, exact, f_paise, err, (double)err / n);
    }

    const long reps = 100000000L;
    volatile float fv = 0.0f;
    volatile long lv = 0;
    clock_t t0 = clock();
    for (long i = 0; i < reps; i++) fv += 0.01f;
    clock_t t1 = clock();
    for (long i = 0; i < reps; i++) lv += 1;
    clock_t t2 = clock();
    printf("\ntime per addition: float %.3f ns, long %.3f ns (both O(1))\n",
           1e9 * (double)(t1 - t0) / CLOCKS_PER_SEC / reps, 1e9 * (double)(t2 - t1) / CLOCKS_PER_SEC / reps);
    printf("space per value  : float %zu bytes, long %zu bytes\n", sizeof(float), sizeof(long));
    printf("100,000,000 additions of 0.01f give %.2f (exact 1000000.00) -> float stops growing once the\n"
           "sum is large enough that 0.01 is below half of one float step.\n", (double)fv);
    return 0;
}
