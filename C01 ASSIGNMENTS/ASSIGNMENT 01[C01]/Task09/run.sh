#!/bin/bash
# Task 09 - build, run the table-driven tests, run the billing program on the input files,
# and run the slab-array alternative.
cd "$(dirname "$0")" || exit 1
F="-std=c11 -Wall -Wextra"
{
  echo "\$ gcc $F -o billing.out main.c billing.c"
  gcc $F -o billing.out main.c billing.c
  echo "\$ gcc $F -o test_billing.out test_billing.c billing.c"
  gcc $F -o test_billing.out test_billing.c billing.c
  echo; echo "\$ ./test_billing.out"
  ./test_billing.out; echo "(exit status $?)"
  echo; echo "\$ ./billing.out < input_hand5.txt      # 5 bills, total calculated by hand = Rs 1487.50"
  ./billing.out < input_hand5.txt
  echo; echo "\$ ./billing.out < input_all_cases.txt  # normal, boundary, roll-over and invalid input"
  ./billing.out < input_all_cases.txt
} 2>&1 | tee output_task09.txt

{
  echo "\$ gcc $F -o alt_slab_array.out alt_slab_array.c billing.c"
  gcc $F -o alt_slab_array.out alt_slab_array.c billing.c
  echo "\$ ./alt_slab_array.out   (unoptimised build)"
  ./alt_slab_array.out
  echo; echo "\$ gcc -std=c11 -O2 -o alt_slab_array_O2.out alt_slab_array.c billing.c"
  gcc -std=c11 -O2 -o alt_slab_array_O2.out alt_slab_array.c billing.c
  echo "\$ ./alt_slab_array_O2.out"
  ./alt_slab_array_O2.out
} 2>&1 | tee output_task09_alt_slab_array.txt
