#!/bin/bash

N=${1:-10}

make exectime-perf INPUT=R N="$N"
make exectime-perf INPUT=S N="$N"
make exectime-perf INPUT=V N="$N"