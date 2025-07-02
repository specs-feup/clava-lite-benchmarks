#!/bin/bash

INPUTS=("sat.trn 4435 37 R" "sat.trn 4435 37 V" "sat.trn 4435 37 S")  # ← your list of subdirectory names

for input_name in "${INPUTS[@]}"; do
    echo "=== Running: make run INPUT=$input_name ==="
    make run INPUT="$input_name"
    echo
done