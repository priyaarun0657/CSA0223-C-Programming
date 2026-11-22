#!/bin/bash
# Task 06 - scanf program on valid and invalid input, fgets alternative (diff'ed against it),
# and the wrong-specifier experiment.
cd "$(dirname "$0")" || exit 1
F="-std=c11 -Wall -Wextra"
{
  echo "\$ gcc $F -o task06_billing_io.out task06_billing_io.c"
  gcc $F -o task06_billing_io.out task06_billing_io.c
  echo; echo "\$ ./task06_billing_io.out < input_valid.txt"
  ./task06_billing_io.out < input_valid.txt
  echo; echo "\$ ./task06_billing_io.out < input_invalid.txt"
  ./task06_billing_io.out < input_invalid.txt
} 2>&1 | tee output_task06_billing_io.txt

{
  echo "\$ gcc $F -o task06_alt_fgets.out task06_alt_fgets.c"
  gcc $F -o task06_alt_fgets.out task06_alt_fgets.c
  echo; echo "\$ ./task06_alt_fgets.out < input_valid.txt"
  ./task06_alt_fgets.out < input_valid.txt
  echo; echo "\$ ./task06_alt_fgets.out < input_invalid.txt"
  ./task06_alt_fgets.out < input_invalid.txt
  echo
  for f in input_valid.txt input_invalid.txt; do
    if diff <(./task06_billing_io.out < $f) <(./task06_alt_fgets.out < $f) > /dev/null; then
      echo "diff scanf-version vs fgets-version on $f : identical output"
    else
      echo "diff scanf-version vs fgets-version on $f : DIFFERENT"
      diff <(./task06_billing_io.out < $f) <(./task06_alt_fgets.out < $f)
    fi
  done
} 2>&1 | tee output_task06_alt_fgets.txt

{
  echo "\$ gcc $F -o task06_wrong_specifier.out task06_wrong_specifier.c      (warnings expected)"
  gcc $F -o task06_wrong_specifier.out task06_wrong_specifier.c
  echo; echo "\$ ./task06_wrong_specifier.out"
  ./task06_wrong_specifier.out
} 2>&1 | tee output_task06_wrong_specifier.txt
