#!/bin/bash
# Task 01 - compile and run both programs; output is saved next to the source.
cd "$(dirname "$0")" || exit 1
source ../common.sh
build_run task01_data_items        2>&1 | tee output_task01_data_items.txt
build_run task01_alt_text_readings 2>&1 | tee output_task01_alt_text_readings.txt
