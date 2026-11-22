#!/bin/bash
# Task 05 - header file + include guard + DEBUG mode, and the run-time-flag alternative.
cd "$(dirname "$0")" || exit 1
F="-std=c11 -Wall -Wextra"
{
  echo "\$ gcc $F -o billing_release.out main.c billing.c"
  gcc $F -o billing_release.out main.c billing.c && ./billing_release.out
  echo
  echo "\$ gcc $F -DDEBUG -o billing_debug.out main.c billing.c"
  gcc $F -DDEBUG -o billing_debug.out main.c billing.c && ./billing_debug.out
  echo
  echo "\$ gcc $F -o alt_runtime_debug.out alt_runtime_debug.c"
  gcc $F -o alt_runtime_debug.out alt_runtime_debug.c
  echo "\$ ./alt_runtime_debug.out        (debug off)"; ./alt_runtime_debug.out
  echo "\$ ./alt_runtime_debug.out -d     (debug on)";  ./alt_runtime_debug.out -d
  echo
  echo "=== include guard evidence: header included twice, compiles without redefinition errors ==="
  printf '#include "billing.h"\n#include "billing.h"\nint main(void){return (int)(FIXED_CHARGE_PAISE - 5000L);}\n' > guard_test.c
  gcc $F -o guard_test.out guard_test.c && ./guard_test.out && echo "OK: guard_test compiled and ran (exit status $?)"
  echo
  echo "=== space: size of the executables (text = machine code) ==="
  size billing_release.out billing_debug.out alt_runtime_debug.out
  echo "debug strings present in executable?"
  printf '  billing_release.out : %s\n' "$(strings billing_release.out | grep -c DEBUG) lines containing DEBUG"
  printf '  billing_debug.out   : %s\n' "$(strings billing_debug.out   | grep -c DEBUG) lines containing DEBUG"
  printf '  alt_runtime_debug   : %s\n' "$(strings alt_runtime_debug.out | grep -c DEBUG) lines containing DEBUG"
  echo
  echo "=== time per bill (gcc -O2, 100,000,000 bills) ==="
  gcc -std=c11 -O2 -o billing_release_O2.out main.c billing.c
  gcc -std=c11 -O2 -o alt_runtime_debug_O2.out alt_runtime_debug.c
  ./billing_release_O2.out --bench
  ./alt_runtime_debug_O2.out --bench
} 2>&1 | tee output_task05.txt
