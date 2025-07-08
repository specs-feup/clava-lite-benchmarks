#!/bin/bash

DATA_DIR="data"
COMP=/tools/Xilinx/Vitis/2024.2/gnu/aarch64/lin/aarch64-linux/bin/aarch64-linux-gnu-gcc

make CC=$COMP INPUT=R
make CC=$COMP INPUT=S
make CC=$COMP INPUT=V
