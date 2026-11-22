#include <stdio.h>
#include <limits.h>

/* The meter register is a 16-bit counter: after 65535 it rolls over to 0. */
typedef unsigned short meter_t;

unsigned int consumption(meter_t previous, meter_t current)
{
    /* TODO 1 (completed): current and previous are promoted to int, so
     * current - previous can be negative (120 - 65400 = -65280). Converting
     * that result back to the 16-bit unsigned type meter_t reduces it modulo
     * 65536 (-65280 + 65536 = 256), which is exactly the number of units
     * counted across the roll-over. No if-else is needed. */
    return (meter_t)(current - previous);
}

int main(void)
{
    printf("sizeof(meter_t) = %zu bytes, maximum reading = %u\n",
           sizeof(meter_t), (unsigned)USHRT_MAX);
    printf("Consumption: %u units\n", consumption(1200, 1530));
    printf("Consumption: %u units\n", consumption(65400, 120)); /* rolled over */
    return 0;
}
