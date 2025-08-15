#!/bin/bash

make assess_acceps || exit 1

. tool.shinc 

recreate=1
maxdr2=0.16
tag="0p4" # 0p5
ver=""

# TYPES=(12 13 14 23 24 34 56 57 67 11 22 33 44 55 66 77)
TYPES=(12 13 14 23 24 34 56 57 67)

# apply
INPUTS_MC=(
    # after maskf
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250809_randomvz_pixel_250809_MINI_0731_Hijing_OO_5362GeV_pf_realistic_maskf.root,hijing
    # after mask
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250727_randomvz_pixel_250727_Hijing_OO_5362GeV_pf_realistic_mask.root,hijing
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250718_randomvz_pixel_250715_Hijing_OO_5362GeV_pf_realistic.root,hijing
)

INPUTS_DATA=(
    # after maskf
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250809_randomvz_pixel_250809_IonPhysics0_394153_maskf_p10.root,394153
    # after mask
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250727_randomvz_pixel_250727_IonPhysics0_394153_mask_p10.root,394153
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250709_randomvz_pixel_250708_IonPhysics0_394153_p10.root,394153
)

outputdir=output/acceptances/${ver}/drlt$tag
mkdir -p $outputdir

for ii in ${INPUTS_DATA[@]}
do
    IFS=',' ; VINPUTS_DATA=($ii) ; unset IFS ;
    INPUT_DATA=${VINPUTS_DATA[0]}
    TAG_DATA=${VINPUTS_DATA[1]}

    echo $INPUT_DATA
    echo "--> "$TAG_DATA

    for jj in ${INPUTS_MC[@]}
    do
        IFS=',' ; VINPUTS_MC=($jj) ; unset IFS ;
        INPUT_MC=${VINPUTS_MC[0]}
        TAG_MC=${VINPUTS_MC[1]}

        echo $INPUT_MC
        echo "--> "$TAG_MC

        for t in ${TYPES[@]}
        do
            ./assess_acceps $recreate $t $maxdr2 $INPUT_DATA $INPUT_MC "$outputdir" ${tag} "$(ftaglabel $TAG_DATA)" "$(ftaglabel $TAG_MC)" &
            # ./assess_acceps $recreate $t $maxdr2 $INPUT_DATA $INPUT_MC "output/acceptances/drlt${TAG_DATA}-${TAG_MC}_${tag}" ${TAG_DATA}-${TAG_MC}_${tag} &
        done
        wait

    done
done

