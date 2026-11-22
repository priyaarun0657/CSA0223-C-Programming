/*
 * Smart-meter billing: ./smart_billing.out <tariff file> <readings file>
 * Prints the usage and the itemised bill for one household's billing period.
 */
#include <stdio.h>
#include <inttypes.h>
#include "smart.h"

static void print_rs(const char *label, paise_t p)
{
    printf("  %-26s Rs %" PRId64 ".%02" PRId64 "\n", label, p / 100, p % 100);
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "usage: %s <tariff file> <readings file>\n", argv[0]);
        return 2;
    }
    char err[160];
    tariff_t tariff;
    usage_t usage;

    if (load_tariff(argv[1], &tariff, err, sizeof err) != 0) { printf("TARIFF ERROR: %s\n", err); return 1; }
    if (read_usage(argv[2], &tariff, &usage, err, sizeof err) != 0) { printf("READINGS ERROR: %s\n", err); return 1; }

    paise_t bill;
    int rc = compute_bill_wh(&tariff, usage.total_wh, usage.peak_wh, &bill);
    if (rc != 0) { printf("BILL ERROR: code %d\n", rc); return 1; }

    /* the same bill WITHOUT the surcharge, to show what the peak rule added */
    paise_t no_peak;
    compute_bill_wh(&tariff, usage.total_wh, 0, &no_peak);

    printf("File %s\n", argv[2]);
    printf("  readings               : %" PRIu32 " (%.1f days)\n", usage.readings, usage.readings / (double)SLOTS_PER_DAY);
    printf("  total energy           : %" PRIu64 " Wh = %" PRIu64 ".%03" PRIu64 " kWh\n",
           usage.total_wh, usage.total_wh / 1000, usage.total_wh % 1000);
    printf("  peak (%02" PRId64 ":00-%02" PRId64 ":00) energy: %" PRIu64 " Wh\n",
           tariff.peak_start_hour, tariff.peak_end_hour, usage.peak_wh);
    print_rs("fixed charge", tariff.fixed_paise);
    print_rs("energy charge (no peak rule)", no_peak - tariff.fixed_paise);
    print_rs("peak surcharge", bill - no_peak);
    print_rs("TOTAL BILL", bill);
    return 0;
}
