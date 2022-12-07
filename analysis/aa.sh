#!/bin/bash

make assess_acceps || exit 1

recreate=0
maxdr2=0.25
tag_maxdr2="drlt0p5"

TYPES=(12 13 14 23 24 34 56 57 67)

# apply
INPUTS=(
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rRun2022/tt_221204_pixelpre_221201_HITestRaw0_HIRun2022A_MBPVfilTh4_362294.root,/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rHydjet/tt_221204_vzshift_pixelpre_20221201_MB_Hydjet_Run3_subehera_Th4.root,362294vHydjet
)

for ii in ${INPUTS[@]}
do
    IFS=',' ; VINPUTS=($ii) ; unset IFS ;
    INPUTDATA=${VINPUTS[0]}
    INPUTMC=${VINPUTS[1]}
    TAG=${VINPUTS[2]}_${tag_maxdr2}

    echo ${TAG}
    for t in ${TYPES[@]}
    do
        ./assess_acceps $recreate $t $maxdr2 $INPUTDATA $INPUTMC "output/acceptances/${tag_maxdr2}" "$TAG" &
    done
    wait
done
