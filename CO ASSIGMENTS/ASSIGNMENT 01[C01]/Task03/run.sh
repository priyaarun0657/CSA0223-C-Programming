#!/bin/bash
# Task 03 - complete-the-code program, the "change RATE2_PAISE to 230" check,
# and the const/enum alternative.
cd "$(dirname "$0")" || exit 1
source ../common.sh
build_run task03_constants 2>&1 | tee output_task03_constants.txt

# Tariff revision: edit ONE line (RATE2_PAISE 225 -> 230) in a copy and re-run.
sed 's|#define RATE2_PAISE 225 /\* Rs 2.25 per unit \*/|#define RATE2_PAISE 230 /* Rs 2.30 per unit */|' task03_constants.c > task03_constants_rate230.c
{
  echo "--- the only difference between the two sources:"
  diff task03_constants.c task03_constants_rate230.c
  build_run task03_constants_rate230
} 2>&1 | tee output_task03_rate230.txt

build_run task03_alt_const_enum 2>&1 | tee output_task03_alt_const_enum.txt
