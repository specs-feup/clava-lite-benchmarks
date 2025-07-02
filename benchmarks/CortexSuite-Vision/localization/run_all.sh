#!/bin/bash

DATA_DIR="data"

for dir in "$DATA_DIR"/*/; do
    input_name=$(basename "$dir")

    if [ "$input_name" = "GroundData" ]; then
        echo "Skipping $input_name"
        continue
    fi

    echo "=== Running: make run INPUT=$input_name ==="
    make run INPUT="$input_name"
    make clean INPUT="$input_name"
    echo
done
