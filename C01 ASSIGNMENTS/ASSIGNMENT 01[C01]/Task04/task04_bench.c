/*
 * Task 04 - time cost of MAX as a macro (-DUSE_MACRO) versus a static inline
 * function (default). Build with -O0 and -O2 to see when the call cost matters.
 */
#include <stdio.h>
#include <time.h>

#define MAX_MACRO(a, b) ((a) > (b) ? (a) : (b))
static inline int max_fn(int a, int b) { return a > b ? a : b; }

#ifdef USE_MACRO
#  define MAX(a, b) MAX_MACRO(a, b)
#  define KIND "macro"
#else
#  define MAX(a, b) max_fn(a, b)
#  define KIND "inline function"
#endif

int main(void)
{
    const long N = 400000000L;
    long sum = 0;
    clock_t t0 = clock();
    for (long i = 0; i < N; i++)
        sum += MAX((int)(i & 1023), 512);
    clock_t t1 = clock();
    printf("%-16s : sum=%ld  %.3f ns per use\n", KIND, sum, 1e9 * (double)(t1 - t0) / CLOCKS_PER_SEC / N);
    return 0;
}
