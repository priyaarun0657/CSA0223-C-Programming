#!/bin/bash
cd "$(dirname "$0")" || exit 1
{
gcc -std=c11 -Wall -Wextra -o smart_billing.out smart_main.c smart.c || exit 1
for h in A B C; do ./smart_billing.out tariff12.cfg readings_house_$h.txt; echo; done
echo "--- revised tariff (no recompile)"; ./smart_billing.out tariff12_revised.cfg readings_house_A.txt; echo
echo "--- invalid files"
for f in text range too_long; do ./smart_billing.out tariff12.cfg readings_invalid_$f.txt; done
} 2>&1 | tee output_task12.txt
