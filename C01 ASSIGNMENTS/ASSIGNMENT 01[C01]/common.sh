#!/bin/bash
# Shared helper used by every TaskNN/run.sh
# build_run <source-base-name> [extra gcc flags]  -> compile with the course flags, then run
build_run() {
  local name=$1; shift
  echo "\$ gcc -std=c11 -Wall -Wextra $* -o $name.out $name.c -lm"
  gcc -std=c11 -Wall -Wextra "$@" -o "$name.out" "$name.c" -lm || return 1
  echo "\$ ./$name.out"
  "./$name.out"
}
