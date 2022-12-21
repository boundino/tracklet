#!/bin/bash

make reap_results || exit 1

maxdr2=0.25
tag_maxdr2="drlt0p5"

TYPES=(12 13 14 23 24 34 56 57 67)

# correction
INPUT=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rHydjet/tt_221204_vzshift_pixelpre_20221201_MB_Hydjet_Run3_subehera_Th4.root
for t in ${TYPES[@]}
do
    ./reap_results $t $INPUT "Hydjet_${tag_maxdr2}" 0 20 0 0 0 1 "null" 0 $maxdr2 &
done
wait

# apply
INPUTS=(
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rRun2022/tt_221204_pixelpre_221201_HITestRaw0_HIRun2022A_MBPVfilTh4_362294.root,362294t
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rRun2022/tt_221204_pixelpre_221201_HITestRaw0_HIRun2022A_MBPVfilTh4_362318.root,362318t
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rHydjet/tt_221204_vzshift_pixelpre_20221201_MB_Hydjet_Run3_subehera_Th4_f11.root,HydjetClosure
)

for ii in ${INPUTS[@]}
do
    IFS=',' ; VINPUTS=($ii) ; unset IFS ;
    INPUT=${VINPUTS[0]}
    TAG=${VINPUTS[1]}_${tag_maxdr2}

    echo $INPUT
    echo ${TAG}
    for t in ${TYPES[@]}
    do
        ./reap_results $t $INPUT "$TAG" 0 20 "Hydjet_${tag_maxdr2}" 1 0 1 "Hydjet_${tag_maxdr2}" 0 $maxdr2 ${tag_maxdr2} &
    done
    wait
done

