# CSA02 C Programming - Assignment 1 (CO1) - Grama Jyothi billing

- Student name: <YOUR NAME>
- Register number: <YOUR REGISTER NUMBER>
- Compiler: gcc 13.3.0 (Ubuntu 24.04)
- Compile command: `gcc -std=c11 -Wall -Wextra -o prog prog.c -lm`
- Run: `bash TaskNN/run.sh` in each folder (compiles, runs, saves `output_*.txt`); `bash Task01/run.sh` etc.

| Task | Result |
|---|---|
| 01 | Data items, constants, rules checked (15003 checks, 0 failed); text-vs-int readings ~9.6x slower |
| 02 | consumption(65400,120)=256 via 16-bit wrap; float total off by Rs 18.78 for 4000 bills, paise exact |
| 03 | Bills 50.00/50.00/162.50/275.00/950.00; RATE2=230 changes all consistently |
| 04 | Macro pitfalls shown with gcc -E; inline alternative |
| 05 | billing.h with include guard, -DDEBUG mode |
| 06 | scanf return-value checks; wrong-specifier demo; fgets alternative (more robust) |
| 07 | 7 conversion surprises; float error grows with n |
| 08 | .i/.s/.o/exe stages; compile/link/run-time errors; warnings fixed |
| 09 | 18/18 tests pass; hand total Rs 1487.50 matches |
| 10 | int overflows at bill 51,717; float total off by Rs 289,362.50 at 800,000 bills |
| 11 | int64_t paise design; tariff.cfg alternative |
| 12 | Smart-meter bill with runtime tariff + 20% peak rule (code + sample outputs) |

Screenshots: add your own terminal screenshots to each task folder (outputs are in `output_*.txt`).
