/*
 * Task 10 - Alternative concept: accumulate the district total in a double
 * instead of a long long. Shows where each type stops being exact
 * (overflow for integers, precision loss for double) and the time per addition.
 */
#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <time.h>

static unsigned int state = 12345u;
static unsigned int next_units(void)
{
    state = state * 1664525u + 1013904223u;
    return (state >> 16) % 401u;
}
static long compute_bill(unsigned int u)
{
    long p = 5000L;
    if (u > 200) p += (long)(u - 200) * 450L;
    if (u > 100) p += (long)((u < 200 ? u : 200) - 100) * 225L;
    return p;
}

int main(void)
{
    long long exact = 0;
    double d_paise = 0.0;      /* whole paise held in a double */
    double d_rupees = 0.0;     /* fractional rupees held in a double */
    for (int i = 0; i < 800000; i++) {
        long p = compute_bill(next_units());
        exact    += p;
        d_paise  += (double)p;
        d_rupees += (double)p / 100.0;
    }
    printf("800,000 bills\n");
    printf("  long long paise total        : %lld\n", exact);
    printf("  double (whole paise) total   : %.1f   difference %.1f paise\n", d_paise, d_paise - (double)exact);
    printf("  double (rupees) total x 100  : %.4f   difference %.6f paise\n", d_rupees * 100.0, d_rupees * 100.0 - (double)exact);

    printf("\nWhere each type stops being exact (paise):\n");
    printf("  int       overflows above %d            = Rs %.0f\n", INT_MAX, INT_MAX / 100.0);
    printf("  long long overflows above %lld = Rs %.4e\n", LLONG_MAX, (double)LLONG_MAX / 100.0);
    printf("  double    exact for whole numbers up to 2^53 = %.0f = Rs %.4e\n", pow(2, 53), pow(2, 53) / 100.0);
    printf("  800,000 bills of the worst-case Rs 21,875 = Rs %.3e: far below both limits\n", 800000.0 * 21875.0);
    volatile double big = 9007199254740992.0;   /* 2^53 */
    printf("  proof: 2^53 + 1 in double = %.0f (the +1 is lost: precision loss, no overflow)\n", (double)(big + 1.0));
    printf("  proof: 2^53 + 2 in double = %.0f\n", (double)(big + 2.0));

    const long reps = 100000000L;
    volatile double dv = 0.0;
    volatile long long lv = 0;
    clock_t t0 = clock();
    for (long i = 0; i < reps; i++) dv += 1.0;
    clock_t t1 = clock();
    for (long i = 0; i < reps; i++) lv += 1;
    clock_t t2 = clock();
    printf("\ntime per addition: double %.3f ns, long long %.3f ns\n",
           1e9 * (double)(t1 - t0) / CLOCKS_PER_SEC / reps, 1e9 * (double)(t2 - t1) / CLOCKS_PER_SEC / reps);
    printf("space per variable: double %zu bytes, long long %zu bytes\n", sizeof(double), sizeof(long long));
    return 0;
}
