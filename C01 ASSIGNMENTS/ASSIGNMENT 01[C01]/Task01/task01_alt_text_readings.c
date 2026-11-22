/*
 * Task 01 - Alternative concept: readings stored as TEXT ("01530") instead of
 * integers. Measures the time to convert + compare each reading (O(d) for d
 * digits) and the space per reading in both representations.
 */
#include <stdio.h>
#include <string.h>
#include <time.h>

#define N 2000000          /* number of reading pairs */
#define D 5                /* digits in a 16-bit reading: "65535" */

static unsigned short prev_i[N], curr_i[N];
static char prev_s[N][D + 1], curr_s[N][D + 1];

static unsigned int str_to_uint(const char *s)          /* O(d) */
{
    unsigned int v = 0;
    while (*s) v = v * 10u + (unsigned)(*s++ - '0');
    return v;
}

int main(void)
{
    unsigned int seed = 2024u;
    for (int i = 0; i < N; i++) {
        seed = seed * 1664525u + 1013904223u;
        unsigned short p = (unsigned short)(seed >> 16);
        seed = seed * 1664525u + 1013904223u;
        unsigned short c = (unsigned short)(p + (seed >> 16) % 400u);   /* may wrap */
        prev_i[i] = p;
        curr_i[i] = c;
        snprintf(prev_s[i], sizeof prev_s[i], "%05hu", p);
        snprintf(curr_s[i], sizeof curr_s[i], "%05hu", c);
    }

    unsigned long long sum_i = 0, sum_s = 0;
    long rollovers_i = 0, rollovers_s = 0;

    clock_t t0 = clock();
    for (int i = 0; i < N; i++) {
        if (curr_i[i] < prev_i[i]) rollovers_i++;
        sum_i += (unsigned short)(curr_i[i] - prev_i[i]);
    }
    clock_t t1 = clock();
    for (int i = 0; i < N; i++) {
        if (strcmp(curr_s[i], prev_s[i]) < 0) rollovers_s++;      /* O(d) compare */
        unsigned int c = str_to_uint(curr_s[i]);                  /* O(d) convert */
        unsigned int p = str_to_uint(prev_s[i]);
        sum_s += (unsigned short)(c - p);
    }
    clock_t t2 = clock();

    double ti = 1e9 * (double)(t1 - t0) / CLOCKS_PER_SEC / N;
    double ts = 1e9 * (double)(t2 - t1) / CLOCKS_PER_SEC / N;
    printf("pairs processed          : %d\n", N);
    printf("integer  total units     : %llu  rollovers: %ld\n", sum_i, rollovers_i);
    printf("text     total units     : %llu  rollovers: %ld\n", sum_s, rollovers_s);
    printf("results identical        : %s\n", (sum_i == sum_s && rollovers_i == rollovers_s) ? "yes" : "NO");
    printf("integer  time per pair   : %.2f ns  (O(1))\n", ti);
    printf("text     time per pair   : %.2f ns  (O(d), d = %d digits)\n", ts, D);
    printf("slowdown of text         : %.1fx\n", ts / ti);
    printf("space per reading        : integer %zu bytes, text %zu bytes (d+1 incl. '\\0')\n",
           sizeof(unsigned short), sizeof prev_s[0]);
    return 0;
}
