#!/bin/bash

DATA_DIR="data"
N=${1:-10}

for dir in "$DATA_DIR"/*/; do
    # Remove trailing slash and path prefix
    input_name=$(basename "$dir")
    
    if [ "$input_name" = "GroundData" ]; then
        echo "Skipping $input_name"
        continue
    fi

    echo "=== Running: make run INPUT=$input_name ==="
    make exectime-time INPUT="$input_name" N="$N"
    echo
done
