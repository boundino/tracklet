#!/bin/bash

make merge_monde || exit 1

CENTS=(20 18 16 14 12 10 8 6)

for c in ${CENTS[@]}
do
    cmin=$((c-1))
    cmax=$c
    label="362294t-Epos_drlt0p5.a."$cmin"."$cmax
    ./merge_monde $label
done
