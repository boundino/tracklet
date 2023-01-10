#!/bin/bash

make assess_acceps || exit 1

recreate=1
maxdr2=0.25
tag="drlt0p5"

TYPES=(12 13 14 23 24 34 56 57 67)
# TYPES=(12)

# apply
INPUTS_MC=(
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_Hydjet_Drum5F_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,Hydjet
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_EposLHC_ReggeGribovParton_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,Epos
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_AMPT_StringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,AMPTstring
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_AMPT_NoStringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,AMPTnostring
)

INPUTS_DATA=(
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221218_pixelpre_221201_HITestRaw0-6_HIRun2022A_MBPVfilTh4_362294.root,362294t
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_EposLHC_ReggeGribovParton_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,EposClose
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_AMPT_StringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,AMPTstringClose
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_AMPT_NoStringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,AMPTnostringClose
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_Hydjet_Drum5F_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,HydjetClose
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
            ./assess_acceps $recreate $t $maxdr2 $INPUT_DATA $INPUT_MC "output/acceptances/${TAG_DATA}-${TAG_MC}_${tag}" ${TAG_DATA}-${TAG_MC}_${tag} &
        done
        wait

    done
done

