/*
 * Task 04 - Alternative concept: static inline functions instead of macros.
 * Arguments are evaluated exactly once and type-checked.
 */
#include <stdio.h>

static inline long rupees_to_paise(long r) { return r * 100L; }
static inline long square_long(long x)     { return x * x; }
static inline int  max_int(int a, int b)   { return a > b ? a : b; }

int main(void)
{
    int a = 4;
    printf("  square_long(a+1) = %ld\n", square_long(a + 1));
    printf("  10 * max_int(2, 3) = %d\n", 10 * max_int(2, 3));

    int i = 5, j = 3;
    int r = max_int(i++, j);
    printf("  max_int(i++, j) with i=5, j=3 -> result %d, i is now %d (i++ evaluated once)\n", r, i);
    printf("  rupees_to_paise(2+3) = %ld paise\n", rupees_to_paise(2 + 3));
    return 0;
}
