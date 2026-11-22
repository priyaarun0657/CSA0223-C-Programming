/*
 * Task 10 - Compare expectations with execution.
 *  A. float / double RUPEES total versus exact PAISE total for 4,000 / 60,000 / 800,000 bills
 *  B. do int, long and long long totals overflow at district scale?
 *  C. sizeof and ranges on this system versus the minimum guaranteed by the C standard
 * Bills use a fixed pseudo-random sequence (0..400 units) so the run is repeatable.
 */
#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <math.h>

static unsigned int state;
static void reset(void) { state = 12345u; }
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
    /* ---------- A ---------- */
    printf("A. totals of n bills: exact paise (long long) versus float and double rupees\n");
    printf("%8s %18s %20s %20s %14s %14s\n", "n", "exact paise", "float rupees", "double rupees", "float error", "double error");
    int counts[] = {4000, 60000, 800000};
    for (int k = 0; k < 3; k++) {
        reset();
        long long exact = 0;
        float  f = 0.0f;
        double d = 0.0;
        for (int i = 0; i < counts[k]; i++) {
            long p = compute_bill(next_units());
            exact += p;
            f += (float)p / 100.0f;
            d += (double)p / 100.0;
        }
        double exact_rupees = (double)exact / 100.0;
        printf("%8d %18lld %20.4f %20.6f %14.4f %14.8f\n", counts[k], exact, (double)f, d,
               (double)f - exact_rupees, d - exact_rupees);
    }
    printf("   (errors are in rupees; the paise total is exact by construction)\n");

    /* ---------- B ---------- */
    printf("\nB. does the district total overflow? (800,000 bills, paise)\n");
    reset();
    int       acc_i  = 0;
    long      acc_l  = 0;
    long long acc_ll = 0;
    long first_i = -1, first_l = -1, first_ll = -1;
    long long exact = 0;
    for (long k = 1; k <= 800000L; k++) {
        long p = compute_bill(next_units());
        exact += p;
        if (first_i  < 0 && __builtin_add_overflow(acc_i,  (int)p,       &acc_i))  first_i  = k;
        if (first_l  < 0 && __builtin_add_overflow(acc_l,  p,            &acc_l))  first_l  = k;
        if (first_ll < 0 && __builtin_add_overflow(acc_ll, (long long)p, &acc_ll)) first_ll = k;
    }
    printf("  exact district total      = %lld paise = Rs %lld.%02lld\n", exact, exact / 100, exact % 100);
    printf("  int       (%zu bytes, max %d)          : %s\n", sizeof(int), INT_MAX,
           first_i  < 0 ? "no overflow" : "OVERFLOWS");
    if (first_i >= 0) printf("            first overflow at bill number %ld\n", first_i);
    printf("  long      (%zu bytes, max %ld) : %s\n", sizeof(long), LONG_MAX,
           first_l  < 0 ? "no overflow on this system" : "OVERFLOWS");
    if (first_l >= 0) printf("            first overflow at bill number %ld\n", first_l);
    printf("  long long (%zu bytes, max %lld) : %s\n", sizeof(long long), LLONG_MAX,
           first_ll < 0 ? "no overflow" : "OVERFLOWS");
    printf("  NOTE: the C standard only guarantees long >= 32 bits. Where long is 32-bit (Windows) it\n"
           "        behaves like int above and overflows. A long long total is safe everywhere.\n");

    /* ---------- C ---------- */
    printf("\nC. this system versus the minimum guaranteed by the C standard\n");
    printf("%-12s %6s %10s %14s   %s\n", "type", "bytes", "bits here", "standard min", "verdict");
    printf("%-12s %6zu %10zu %14s   %s\n", "char",      sizeof(char),      sizeof(char)      * CHAR_BIT, "8 bits",  "equal to the minimum");
    printf("%-12s %6zu %10zu %14s   %s\n", "short",     sizeof(short),     sizeof(short)     * CHAR_BIT, "16 bits", sizeof(short)     * CHAR_BIT == 16 ? "equal to the minimum" : "larger than the minimum");
    printf("%-12s %6zu %10zu %14s   %s\n", "int",       sizeof(int),       sizeof(int)       * CHAR_BIT, "16 bits", sizeof(int)       * CHAR_BIT == 16 ? "equal to the minimum" : "larger than the minimum");
    printf("%-12s %6zu %10zu %14s   %s\n", "long",      sizeof(long),      sizeof(long)      * CHAR_BIT, "32 bits", sizeof(long)      * CHAR_BIT == 32 ? "equal to the minimum" : "larger than the minimum");
    printf("%-12s %6zu %10zu %14s   %s\n", "long long", sizeof(long long), sizeof(long long) * CHAR_BIT, "64 bits", sizeof(long long) * CHAR_BIT == 64 ? "equal to the minimum" : "larger than the minimum");
    printf("%-12s %6zu %10s %14s   FLT_DIG = %d  %s\n", "float",  sizeof(float),  "-", "6 digits",  FLT_DIG, FLT_DIG >= 6   ? "(meets the minimum)" : "(BELOW)");
    printf("%-12s %6zu %10s %14s   DBL_DIG = %d %s\n", "double", sizeof(double), "-", "10 digits", DBL_DIG, DBL_DIG >= 10 ? "(meets the minimum)" : "(BELOW)");
    printf("  USHRT_MAX = %u (standard guarantees at least 65535: the 16-bit meter register fits)\n", (unsigned)USHRT_MAX);
    printf("  INT_MAX = %d (standard guarantees at least 32767)\n", INT_MAX);
    printf("  LONG_MAX = %ld (standard guarantees at least 2147483647)\n", LONG_MAX);
    return 0;
}
