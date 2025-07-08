#!/bin/bash

DATA_DIR="data"
N=${1:-10}

for dir in "$DATA_DIR"/*/; do
    # Remove trailing slash and path prefix
    input_name=$(basename "$dir")
    

    echo "=== Running: make run INPUT=$input_name ==="
    make exectime-time INPUT="$input_name" N="$N"
    echo
done
