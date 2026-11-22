#!/bin/bash
# Task 02 - compile and run the three programs; outputs are saved.
cd "$(dirname "$0")" || exit 1
source ../common.sh
build_run task02_consumption 2>&1 | tee output_task02_consumption.txt
build_run task02_datatypes   2>&1 | tee output_task02_datatypes.txt
build_run task02_alt_double  2>&1 | tee output_task02_alt_double.txt
