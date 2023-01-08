#!/bin/bash

make assess_acceps || exit 1

recreate=1
maxdr2=0.25
tag="drlt0p5"

TYPES=(12 13 14 23 24 34 56 57 67)
# TYPES=(12)

# apply
INPUTS=(
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221218_pixelpre_221201_HITestRaw0-6_HIRun2022A_MBPVfilTh4_362294.root,/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_Hydjet_Drum5F_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,fine200
)

for ii in ${INPUTS[@]}
do
    IFS=',' ; VINPUTS=($ii) ; unset IFS ;
    INPUTDATA=${VINPUTS[0]}
    INPUTMC=${VINPUTS[1]}
    TAG=${VINPUTS[2]}_${tag}

    echo ${TAG}
    for t in ${TYPES[@]}
    do
        ./assess_acceps $recreate $t $maxdr2 $INPUTDATA $INPUTMC "output/acceptances/${tag}" $TAG &
    done
    wait
done
