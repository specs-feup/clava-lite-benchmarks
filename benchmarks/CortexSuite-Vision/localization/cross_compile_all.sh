#!/bin/bash

DATA_DIR="data"
COMP=/tools/Xilinx/Vitis/2024.2/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-gcc

for dir in "$DATA_DIR"/*/; do
    # Remove trailing slash and path prefix
    input_name=$(basename "$dir")

    if [ "$input_name" = "GroundData" ]; then
        echo "Skipping $input_name"
        continue
    fi
    
    make CC=$COMP INPUT="$input_name"
done
