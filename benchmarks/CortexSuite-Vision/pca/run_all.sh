#!/bin/bash
N=${1:-10}

echo "=== Running: make run INPUT=R ==="
make run-perf INPUT="R" N="$N"
echo

echo "=== Running: make run INPUT=R ==="
make run-perf INPUT="V" N="$N"
echo

echo "=== Running: make run INPUT=R ==="
make run-perf INPUT="S" N="$N"
echo