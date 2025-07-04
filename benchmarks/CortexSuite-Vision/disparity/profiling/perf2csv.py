import csv
import re

input_file = "perf-flat.txt"
output_file = "perf-profile.csv"

# Matches:
# --10.02%--0xFFFFFFFF867E2D01
# --0.02%--__GI__IO_sputbackc (inlined)
# --0.01%--readImage
pattern = re.compile(r"^[| ]*--([0-9]+\.[0-9]+)%--([0-9a-zA-Z_ ()]+)")

with open(input_file, "r") as infile, open(output_file, "w", newline="") as outfile:
    writer = csv.writer(outfile)
    writer.writerow(["percent", "function"])

    for line in infile:
        match = pattern.match(line)
        if match:
            print(match)
            percent, function = match.groups()
            writer.writerow([percent, function])
    s = "--0.00%--__GI__IO_sputbackc (inlined)"
    match = pattern.match(s)
    if match:
        percent, function = match.groups()
        writer.writerow([percent, function])
