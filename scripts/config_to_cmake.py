#!/usr/bin/env python3
import sys

if len(sys.argv) != 3:
    print(f"Usage: {sys.argv[0]} <infile> <outfile>")

with open(sys.argv[1]) as in_file, open(sys.argv[2], "w") as out_file:
    for line in in_file:
        # Check for unknown line
        if not line[0].isupper():  # comment
            out_file.write(line)
            continue

        # Actually deal with the config line
        sym, val = line.strip().split("=", 1)

        match val[0]:
            case "y":  # bool true
                out_file.write(f"set({sym} TRUE)")

            case "m":  # module???
                sys.exit(99)

            case "n":  # bool false
                out_file.write(f"set({sym} FALSE)")

            case _:  # string or number
                out_file.write(f"set({sym} {val})")

        out_file.write("\n")
