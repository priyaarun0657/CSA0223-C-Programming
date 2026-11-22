#!/bin/bash
# Task 10 - measured comparison of float / paise / overflow behaviour.
cd "$(dirname "$0")" || exit 1
source ../common.sh
build_run task10_expectation_vs_execution 2>&1 | tee output_task10.txt
build_run task10_alt_double_total         2>&1 | tee output_task10_alt_double_total.txt
