#!/bin/bash
# Task 07 - conversion surprises and the float-versus-paise error measurement.
cd "$(dirname "$0")" || exit 1
source ../common.sh
build_run task07_conversions     2>&1 | tee output_task07_conversions.txt
build_run task07_alt_float_error 2>&1 | tee output_task07_alt_float_error.txt
