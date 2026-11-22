#!/bin/bash
# Task 11 - recommended design, tariff revision by editing ONE header line, and the config-file alternative.
cd "$(dirname "$0")" || exit 1
F="-std=c11 -Wall -Wextra"
{
  echo "\$ gcc $F -o task11_final_demo.out task11_final_demo.c"
  gcc $F -o task11_final_demo.out task11_final_demo.c && ./task11_final_demo.out

  echo; echo "=== TARIFF REVISION, compiled-in design: edit one line of final_design.h, recompile, redistribute ==="
  rm -rf revision && mkdir revision
  cp task11_final_demo.c revision/
  sed 's|#define RATE2_PAISE         ((paise_t)225)|#define RATE2_PAISE         ((paise_t)230)|' final_design.h > revision/final_design.h
  echo "--- the only change:"; diff final_design.h revision/final_design.h
  s=$(date +%s%N)
  gcc $F -o revision/task11_final_demo.out revision/task11_final_demo.c
  e=$(date +%s%N)
  ./revision/task11_final_demo.out | head -6
  echo "recompile time: $(( (e - s) / 1000000 )) ms (plus copying the new executable to every office computer)"
} 2>&1 | tee output_task11_final_design.txt

{
  echo "\$ gcc $F -o task11_config_tariff.out task11_config_tariff.c"
  gcc $F -o task11_config_tariff.out task11_config_tariff.c
  echo; echo "\$ ./task11_config_tariff.out tariff.cfg"
  ./task11_config_tariff.out tariff.cfg
  echo; echo "\$ ./task11_config_tariff.out tariff_revised.cfg      # NO recompile: only the text file differs"
  ./task11_config_tariff.out tariff_revised.cfg
  echo; echo "--- invalid configuration is refused, not silently used"
  ./task11_config_tariff.out tariff_invalid.cfg
} 2>&1 | tee output_task11_config_tariff.txt
