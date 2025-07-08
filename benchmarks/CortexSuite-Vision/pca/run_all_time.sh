#!/bin/bash

N=${1:-10}

make exectime-time INPUT=R N="$N"
make exectime-time INPUT=S N="$N"
make exectime-time INPUT=V N="$N"
