#!/bin/bash

echo "=== Running: make run INPUT=R ==="
make run-perf INPUT="R"
echo

echo "=== Running: make run INPUT=R ==="
make run-perf INPUT="V"
echo

echo "=== Running: make run INPUT=R ==="
make run-perf INPUT="S"
echo