#!/bin/bash
# Task 08 - every stage of compilation, classified errors/warnings, and the split-file build.
cd "$(dirname "$0")" || exit 1
F="-std=c11"
mkdir -p stages

{
echo "################ A. THE FOUR STAGES (task08_billing.c)"
echo "\$ gcc $F -E task08_billing.c -o stages/task08_billing.i      # 1. preprocess"
gcc $F -E task08_billing.c -o stages/task08_billing.i
echo "\$ gcc $F -S task08_billing.c -o stages/task08_billing.s      # 2. compile to assembly"
gcc $F -S task08_billing.c -o stages/task08_billing.s
echo "\$ gcc $F -c task08_billing.c -o stages/task08_billing.o      # 3. assemble to object file"
gcc $F -c task08_billing.c -o stages/task08_billing.o
echo "\$ gcc $F stages/task08_billing.o -o stages/task08_billing.out # 4. link to executable"
gcc $F stages/task08_billing.o -o stages/task08_billing.out
echo
echo "--- size of every stage's output"
wc -l task08_billing.c stages/task08_billing.i stages/task08_billing.s
ls -l stages/task08_billing.o stages/task08_billing.out | awk '{print $5, $9}'
echo
echo "--- what the preprocessor replaced or removed (source line  ->  line in the .i file)"
grep -n 'FIXED_CHARGE_PAISE\|SQUARE_PAISE' task08_billing.c | head -4
echo "  becomes:"
grep -n 'long paise = 5000L\|((p / 100) \* 100L)' stages/task08_billing.i
echo "  comments in source: $(grep -c '/\*' task08_billing.c)   comments in .i file: $(grep -c 'comment:' stages/task08_billing.i)"
echo "  #include <stdio.h> pulled in $(grep -c . stages/task08_billing.i) non-empty lines in total (source had $(grep -c . task08_billing.c))"
echo
echo "--- start of the assembly (.s) for compute_bill"
grep -n -A12 '^compute_bill:' stages/task08_billing.s
echo
echo "--- symbols in the object file (U = undefined, resolved by the linker from libc)"
nm stages/task08_billing.o
echo
echo "--- run the executable"
./stages/task08_billing.out
} 2>&1 | tee output_task08_stages.txt

{
echo "################ B. ERRORS, CLASSIFIED"
cd errors
echo; echo "--- COMPILE-TIME error"
echo "\$ gcc $F -c compile_error.c"
gcc $F -c compile_error.c
echo "exit status: $?"
echo; echo "--- LINK-TIME error (compilation to .o succeeds, linking fails)"
echo "\$ gcc $F -c link_error.c -o link_error.o"
gcc $F -c link_error.c -o link_error.o && echo "compile step OK"
echo "\$ gcc $F link_error.o -o link_error.out"
gcc $F link_error.o -o link_error.out
echo "exit status: $?"
echo; echo "--- RUN-TIME error (compiles and links, fails while running)"
echo "\$ gcc $F -Wall -Wextra -o runtime_error.out runtime_error.c"
gcc $F -Wall -Wextra -o runtime_error.out runtime_error.c
echo "\$ ./runtime_error.out"
bash -c './runtime_error.out; echo "exit status: $?  (136 = 128 + signal 8, SIGFPE)"' 2>&1
cd ..
} 2>&1 | tee output_task08_errors.txt

{
echo "################ C. WARNINGS (-Wall -Wextra) AND THEIR FIX"
cd errors
echo "\$ gcc $F -Wall -Wextra -c warn_demo.c -o warn_demo.o"
gcc $F -Wall -Wextra -c warn_demo.c -o warn_demo.o
echo; echo "\$ gcc $F -Wall -Wextra -c warn_fixed.c -o warn_fixed.o"
gcc $F -Wall -Wextra -c warn_fixed.c -o warn_fixed.o && echo "no warnings"
cd ..
} 2>&1 | tee output_task08_warnings.txt

{
echo "################ D. ALTERNATIVE: several .c files + shared header + Makefile"
cd alt_split
make clean > /dev/null
echo "\$ make"
make
echo "\$ ./billing_split.out"
./billing_split.out
echo
echo "--- space used by the object files and the executable"
size input.o tariff.o report.o billing_split.out
ls -l input.o tariff.o report.o | awk '{print $5, $9}'

# single-file equivalent: header + the three sources concatenated
{ cat billing8.h; sed '/#include "billing8.h"/d' input.c tariff.c report.c; } > all_in_one.c

avg_ms() {   # avg_ms <repetitions> <command...>
  local reps=$1; shift
  local t0 t1
  t0=$(date +%s%N)
  for ((i = 0; i < reps; i++)); do "$@" > /dev/null 2>&1; done
  t1=$(date +%s%N)
  echo $(( (t1 - t0) / reps / 1000 ))
}
full_split()   { make clean >/dev/null; make >/dev/null; }
incr_split()   { touch tariff.c; make >/dev/null; }
single_build() { gcc -std=c11 -Wall -Wextra -o all_in_one.out all_in_one.c; }
echo
echo "--- build time in microseconds (average of 20 builds, tiny programs)"
echo "single file, any change (recompile everything + link) : $(avg_ms 20 single_build)"
echo "split files, full rebuild (3 compiles + link)         : $(avg_ms 20 full_split)"
echo "split files, ONE file changed (1 compile + link)      : $(avg_ms 20 incr_split)"
make clean > /dev/null
cd ..
} 2>&1 | tee output_task08_split_build.txt
