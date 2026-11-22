/*
 * Task 02 - Choose the data types.
 * Prints sizeof and ranges (limits.h / float.h), checks whether each type can
 * hold a district total, shows the 16-bit meter roll-over, and shows float
 * giving a wrong total for 4,000 bills while integer paise stays exact.
 */
#include <stdio.h>
#include <limits.h>
#include <float.h>

#define S(t)  sizeof(t)

int main(void)
{
    printf("%-20s %-6s %s\n", "TYPE", "BYTES", "RANGE");
    printf("%-20s %-6zu %d .. %d\n",       "char",               S(char),               CHAR_MIN, CHAR_MAX);
    printf("%-20s %-6zu %d .. %d\n",       "short",              S(short),              SHRT_MIN, SHRT_MAX);
    printf("%-20s %-6zu 0 .. %u\n",        "unsigned short",     S(unsigned short),     (unsigned)USHRT_MAX);
    printf("%-20s %-6zu %d .. %d\n",       "int",                S(int),                INT_MIN, INT_MAX);
    printf("%-20s %-6zu 0 .. %u\n",        "unsigned int",       S(unsigned int),       UINT_MAX);
    printf("%-20s %-6zu %ld .. %ld\n",     "long",               S(long),               LONG_MIN, LONG_MAX);
    printf("%-20s %-6zu %lld .. %lld\n",   "long long",          S(long long),          LLONG_MIN, LLONG_MAX);
    printf("%-20s %-6zu 0 .. %llu\n",      "unsigned long long", S(unsigned long long), ULLONG_MAX);
    printf("%-20s %-6zu max %e, %d digits, epsilon %e\n", "float",  S(float),  (double)FLT_MAX, FLT_DIG, (double)FLT_EPSILON);
    printf("%-20s %-6zu max %e, %d digits, epsilon %e\n", "double", S(double), DBL_MAX, DBL_DIG, DBL_EPSILON);

    /* Is each type big enough for a district? Worst case: 800,000 houses,
     * each paying the bill for the maximum plausible 5000 units. */
    long long worst_bill = 5000LL + 100LL * 225 + 4800LL * 450;          /* paise */
    long long district   = 800000LL * worst_bill;
    printf("\nWorst-case bill = %lld paise; worst-case district total = %lld paise\n", worst_bill, district);
    printf("  fits in int       (max %d)?  %s\n", INT_MAX,  district <= INT_MAX  ? "yes" : "NO");
    printf("  fits in long      (max %ld)? %s\n", LONG_MAX, district <= LONG_MAX ? "yes" : "NO");
    printf("  fits in long long (max %lld)? %s\n", LLONG_MAX, district <= LLONG_MAX ? "yes" : "NO");

    /* 16-bit unsigned meter register after 65535 */
    unsigned short reg = 65535;
    reg++;
    printf("\nunsigned short 65535 + 1 -> %hu  (wraps to 0, defined behaviour for unsigned types)\n", reg);

    /* float vs integer paise: 4000 bills of Rs 123.45 each */
    float f_total = 0.0f;
    long  p_total = 0;
    for (int i = 0; i < 4000; i++) {
        f_total += 123.45f;
        p_total += 12345L;
    }
    printf("\n4000 bills of Rs 123.45 each (exact total Rs 493800.00)\n");
    printf("  float  rupees total : %.4f\n", f_total);
    printf("  float  error        : %.4f rupees\n", (double)f_total - 493800.0);
    printf("  paise  integer total: %ld  -> Rs %ld.%02ld  (exact)\n", p_total, p_total / 100, p_total % 100);
    return 0;
}
