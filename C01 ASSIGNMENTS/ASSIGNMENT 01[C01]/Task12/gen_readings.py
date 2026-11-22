#!/usr/bin/env python3
"""Generates the sample 15-minute reading files (one Wh value per line, 30 days = 2880 lines).
The files are committed, so this script is only needed to regenerate them."""

SLOTS = 96
DAYS = 30


def peak(slot):                       # 18:00 <= time < 22:00  (slots 72..87)
    return 72 <= slot < 88


def write(name, f):
    with open(name, "w") as out:
        for d in range(DAYS):
            for s in range(SLOTS):
                out.write(f"{f(d, s)}\n")


# A: 150 Wh off-peak, 400 Wh peak per slot  -> 552.000 kWh, 192.000 kWh of it in the peak window
write("readings_house_A.txt", lambda d, s: 400 if peak(s) else 150)
# B: no peak usage at all (60 Wh off-peak, 0 at peak) -> 144.000 kWh: must equal the OLD bill
write("readings_house_B.txt", lambda d, s: 0 if peak(s) else 60)
# C: heavy user, ALL usage in the peak window at 10 kW (2500 Wh per slot) -> 1200.000 kWh
write("readings_house_C.txt", lambda d, s: 2500 if peak(s) else 0)

# D: invalid file - a letter on line 100
with open("readings_invalid_text.txt", "w") as out:
    for i in range(200):
        out.write("12x\n" if i == 99 else "100\n")

# E: invalid file - one 15-minute reading of 6000 Wh (above the 5000 Wh limit)
with open("readings_invalid_range.txt", "w") as out:
    for i in range(200):
        out.write("6000\n" if i == 50 else "100\n")

# F: invalid file - 3000 readings (more than 31 days)
with open("readings_invalid_too_long.txt", "w") as out:
    for i in range(3000):
        out.write("100\n")
