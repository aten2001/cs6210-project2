#!/bin/bash

if [[ $# < 1 ]]; then
    echo "Usage: $0 <barrier_type> <num_threads>"
fi

if [[ $1x == "allx" ]]; then
    for i in $(seq 0 6); do
        ./omp_timing $i $2
    done
fi
