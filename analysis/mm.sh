#!/bin/bash

make merge_monde || exit 1

tagver="v1"
CENTS=(
    # 10 11
    # 19 20
    4 20
)

label="362294.cgm.epos.m."$tagver".s"
text="Run 362294 corr. w. EPOS"
c=0
while [ $c -lt $((${#CENTS[@]}-1)) ] ; do
    cmin=${CENTS[c]}
    cmax=${CENTS[c+1]}
    # truth="epos.m.v1.s."$cmin"."$cmax"&EPOS LHC&2,hydjet.m.v1.s."$cmin"."$cmax"&HYDJET&1,amptsm.m.v1.s."$cmin"."$cmax"&AMPT (string melting)&4"
    ./merge_monde $label"."$cmin"."$cmax "$text"

    c=$((c+1))
done
