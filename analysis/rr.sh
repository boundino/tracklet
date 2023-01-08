#!/bin/bash

make reap_results || exit 1
make merge_first || exit 1

maxdr2=0.25
tag="drlt0p5"

TYPES=(12 13 14 23 24 34 56 57 67)
# TYPES=(12)

##
INPUTS_MC=(
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_Hydjet_Drum5F_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,Hydjet
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_EposLHC_ReggeGribovParton_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,Epos
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_AMPT_StringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,AMPTstring
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_AMPT_NoStringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,AMPTnostring
)

INPUTS_DATA=(
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221218_pixelpre_221201_HITestRaw0-6_HIRun2022A_MBPVfilTh4_362294.root,362294t
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_Hydjet_Drum5F_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,HydjetClose
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_EposLHC_ReggeGribovParton_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,EposClose
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_AMPT_StringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,AMPTstringClose
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_AMPT_NoStringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,AMPTnostringClose
)

# correction
for ii in ${INPUTS_MC[@]}
do
    IFS=',' ; VINPUTS=($ii) ; unset IFS ;
    INPUT=${VINPUTS[0]}
    TAG=${VINPUTS[1]}_${tag}

    echo $INPUT
    echo $TAG
    [[ ${1:-0} -eq 1 ]] && {
        for t in ${TYPES[@]}
        do
            # ./reap_results $t $INPUT $TAG 0 20 0 0 0 1 "null" 0 $maxdr2 ${tag} "null" 0 "(vz[1]<5 && vz[1]>-5)" &
            ./reap_results $t $INPUT $TAG 0 20 0 0 0 1 "null" 0 $maxdr2 ${tag} "null" 0 "(1)" &
        done
        wait
    }
done

# apply
for ii in ${INPUTS_DATA[@]}
do
    IFS=',' ; VINPUTS_DATA=($ii) ; unset IFS ;
    INPUT_DATA=${VINPUTS_DATA[0]}
    TAG_DATA=${VINPUTS_DATA[1]}

    echo $INPUT_DATA
    echo $TAG_DATA

    for jj in ${INPUTS_MC[@]}
    do
        IFS=',' ; VINPUTS_MC=($jj) ; unset IFS ;
        INPUT_MC=${VINPUTS_MC[0]}
        TAG_MC=${VINPUTS_MC[1]}_${tag}

        [[ ${2:-0} -eq 1 ]] && {
            for t in ${TYPES[@]}
            do
                # ./reap_results $t $INPUT_DATA ${TAG_DATA}-${TAG_MC} 0 20 $TAG_MC 1 0 1 $TAG_MC 0 $maxdr2 ${tag} "null" 0 "(vz[1]<5 && vz[1]>-5)" &
                # ./reap_results $t $INPUT_DATA ${TAG_DATA}-${TAG_MC} 0 20 $TAG_MC 1 0 1 $TAG_MC 0 $maxdr2 ${tag} "null" 0 "(1)" &
                ./reap_results $t $INPUT_DATA ${TAG_DATA}-${TAG_MC} 0 20 $TAG_MC 1 1 1 $TAG_MC 0 $maxdr2 ${tag} "null" 0 "(1)" &
            done
            wait
        }

        [[ ${3:-0} -eq 1 ]] && {
            ./merge_first output/correction-${TAG_MC}-12.root output/correction-${TAG_DATA}-${TAG_MC} "${TAG_DATA}-${TAG_MC}"
        }

        # for t in ${TYPES[@]}
        # do
        #     ./assess_acceps 1 $t $maxdr2 $INPUT_DATA $INPUT_MC "output/acceptances/${tag}" "$TAG_MC" &
        # done
        # wait
    done
done
