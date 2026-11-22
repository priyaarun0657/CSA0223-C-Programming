/*
 * Program structure: header files -> global declarations -> main() -> other
 * functions (compute_bill lives in billing.c).
 * Build normally            : gcc -std=c11 -Wall -Wextra -o billing_release.out main.c billing.c
 * Build with debugging mode : gcc -std=c11 -Wall -Wextra -DDEBUG -o billing_debug.out main.c billing.c
 * `--bench` runs 100,000,000 bills and prints the time per bill.
 */
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "billing.h"

int main(int argc, char *argv[])
{
    if (argc > 1 && strcmp(argv[1], "--bench") == 0) {
        const long N = 100000000L;
        long sum = 0;
        clock_t t0 = clock();
        for (long i = 0; i < N; i++) sum += compute_bill((int)(i % 400));
        clock_t t1 = clock();
        printf("bench: sum=%ld, %.3f ns per bill\n", sum, 1e9 * (double)(t1 - t0) / CLOCKS_PER_SEC / N);
        return 0;
    }

    int tests[] = {0, 150, 350};
    for (int i = 0; i < 3; i++) {
        long p = compute_bill(tests[i]);
        printf("%3d units -> Rs %ld.%02ld\n", tests[i], p / 100, p % 100);
    }
    return 0;
}
