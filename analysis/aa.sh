#!/bin/bash

make assess_acceps || exit 1

. tool.shinc 

recreate=0
maxdr2=0.25
tag="0p5" # 0p5

# TYPES=(12 13 14 23 24 34 56 57 67 11 22 33 44 55 66 77)
TYPES=(12 13 14 23 24 34 56 57 67)

# apply
INPUTS_MC=(
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_randomvz_pixel_250528_Hijing_OO_5360GeV_0527_v2.root,hijing
    # pre-approval
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230130_vzrandom_pixel_230129_Hydjet_Drum5F_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,hydjet
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230114_vzrandom_pixelpre_221229_EposLHC_ReggeGribovParton_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,epos
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230114_vzrandom_pixelpre_221229_AMPT_StringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,amptsm
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230114_vzrandom_pixelpre_221229_AMPT_NoStringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,amptnm
)

INPUTS_DATA=(
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_randomvz_pixel_250528_AMPT_NoStringMeltingp0_OO_5360GeV_0527_v2.root,amptnm2    
    # /eos/cms/store/cmst3/user/wangj/tracklet/tt_230724_randomvz_pixel_230724_HITestRaw0-6_HIRun2022A_MBPVfilTh4_362294.root,362294a
    # pre-approval
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230130_vzrandom_pixel_230126_HITestRaw0-6_HIRun2022A_MBPVfilTh4_362294.root,362294a
)

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
            ./assess_acceps $recreate $t $maxdr2 $INPUT_DATA $INPUT_MC "output/acceptances/drlt${tag}" ${tag} $taglabel[$TAG_DATA] $taglabel[$TAG_MC] &
            # ./assess_acceps $recreate $t $maxdr2 $INPUT_DATA $INPUT_MC "output/acceptances/drlt${TAG_DATA}-${TAG_MC}_${tag}" ${TAG_DATA}-${TAG_MC}_${tag} &
        done
        wait

    done
done

