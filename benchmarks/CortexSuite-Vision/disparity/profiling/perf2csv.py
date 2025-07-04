import csv
import re

input_file = "perf-flat.txt"
output_file = "perf-profile.csv"

# Matches:
# --0.01%--readImage
# |--10.02%--0xFFFFFFFF867E2D01
#     |     |--0.02%--__GI__IO_sputbackc (inlined)
pattern = re.compile(r"^[| ]*--([0-9]+\.[0-9]+)%--([0-9a-zA-Z_ ()]+)")
hexadecimal = re.compile(r"0x[0-9a-fA-F]+")

with open(input_file, "r") as infile, open(output_file, "w", newline="") as outfile:
    fun_percentages = {}

    for line in infile:
        match = pattern.match(line)
        if match:
            percent, function = match.groups()

            hex_match = hexadecimal.search(function)
            if hex_match:
                continue
            n_percent = float(percent)
            if function not in fun_percentages:
                fun_percentages[function] = n_percent

    writer = csv.writer(outfile)
    writer.writerow(
        [
            "function",
            "percent",
        ]
    )
    for function, percent in fun_percentages.items():
        writer.writerow([function, f"{percent:.2f}"])
