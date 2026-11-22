/*
 * Task 04 - Use macros safely.
 * Correct, fully parenthesised macros next to deliberately BAD versions, so
 * every pitfall can be seen in the output and in `gcc -E`.
 */
#include <stdio.h>

/* ---- correct macros ---- */
#define RUPEES_TO_PAISE(r)  ((long)(r) * 100L)
#define SQUARE(x)           ((x) * (x))
#define MAX(a, b)           ((a) > (b) ? (a) : (b))

/* ---- deliberately wrong macros (missing parentheses) ---- */
#define BAD_SQUARE(x)       x * x
#define BAD_MAX(a, b)       a > b ? a : b

int main(void)
{
    int a = 4;
    printf("Pitfall 1: missing parentheses around the argument\n");
    printf("  BAD_SQUARE(a+1) = %d   (expands to a+1*a+1 = 4+4+1)\n", BAD_SQUARE(a + 1));
    printf("  SQUARE(a+1)     = %d   (expands to ((a+1)*(a+1)))\n", SQUARE(a + 1));

    printf("Pitfall 2: missing parentheses around the whole expansion\n");
    printf("  10 * BAD_MAX(2, 3) = %d  (expected 30)\n", 10 * BAD_MAX(2, 3));
    printf("  10 * MAX(2, 3)     = %d\n", 10 * MAX(2, 3));

    printf("Pitfall 3: an argument with a side effect is evaluated twice\n");
    int i = 5, j = 3;
    int r = MAX(i++, j);
    printf("  MAX(i++, j) with i=5, j=3 -> result %d, i is now %d (i++ ran twice; expected 5 and 6)\n", r, i);

    printf("Useful macro: RUPEES_TO_PAISE\n");
    printf("  RUPEES_TO_PAISE(50)  = %ld paise\n", RUPEES_TO_PAISE(50));
    printf("  RUPEES_TO_PAISE(2+3) = %ld paise  (argument parenthesised, so 500 and not 202)\n", RUPEES_TO_PAISE(2 + 3));
    return 0;
}
