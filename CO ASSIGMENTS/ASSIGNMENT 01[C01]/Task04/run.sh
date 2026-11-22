#!/bin/bash
# Task 04 - macros, gcc -E evidence, inline alternative, time and code-size comparison.
cd "$(dirname "$0")" || exit 1
source ../common.sh
{
  build_run task04_macros
  echo
  echo "=== gcc -E : what the preprocessor really produced for main() ==="
  echo "\$ gcc -std=c11 -E task04_macros.c -o task04_macros.i"
  gcc -std=c11 -E task04_macros.c -o task04_macros.i
  grep -n -E 'a \+ 1 \* a \+ 1|\(\(a \+ 1\) \* \(a \+ 1\)\)|10 \* 2 > 3|10 \* \(\(2\) > \(3\)|i\+\+\) > \(j\)|\(\(long\)\(50\) \* 100L\)|\(\(long\)\(2 \+ 3\) \* 100L\)' task04_macros.i
} 2>&1 | tee output_task04_macros.txt

build_run task04_alt_inline 2>&1 | tee output_task04_alt_inline.txt

{
  echo "=== time per use (400,000,000 uses) ==="
  for opt in -O0 -O2; do
    gcc -std=c11 $opt -DUSE_MACRO -o bench_macro.out task04_bench.c && echo "$opt" && ./bench_macro.out
    gcc -std=c11 $opt            -o bench_inline.out task04_bench.c && ./bench_inline.out
  done

  echo
  echo "=== code size when the operation is used k = 1000 times (gcc -c, 'size' text section) ==="
  { echo '#define MAX(a,b) ((a) > (b) ? (a) : (b))'
    echo 'int f(int *v){ int s=0;'
    for k in $(seq 1 1000); do echo "s += MAX(v[$((k % 50))], v[$(((k+7) % 50))]);"; done
    echo 'return s; }'; } > macro_k1000.c
  { echo 'static inline int max_fn(int a,int b){ return a>b?a:b; }'
    echo 'int f(int *v){ int s=0;'
    for k in $(seq 1 1000); do echo "s += max_fn(v[$((k % 50))], v[$(((k+7) % 50))]);"; done
    echo 'return s; }'; } > inline_k1000.c
  for opt in -O0 -O2; do
    echo "--- $opt"
    gcc -std=c11 $opt -c macro_k1000.c  -o macro_k1000.o
    gcc -std=c11 $opt -c inline_k1000.c -o inline_k1000.o
    size macro_k1000.o inline_k1000.o
  done
} 2>&1 | tee output_task04_compare.txt
