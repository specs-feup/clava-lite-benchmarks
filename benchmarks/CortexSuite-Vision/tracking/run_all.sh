#!/bin/bash

DATA_DIR="data"

for dir in "$DATA_DIR"/*/; do
    # Remove trailing slash and path prefix
    input_name=$(basename "$dir")

    echo "=== Running: make run INPUT=$input_name ==="
    make run INPUT="$input_name"
    echo
done
