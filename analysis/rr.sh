#!/bin/bash

make reap_results || exit 1

nentries=
maxdr2=0.25
tag="drlt0p5"
TYPES=(12 13 14 23 24 34 56 57 67)

##
INPUTS_MC=(
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230130_pixel_230129_Hydjet_Drum5F_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,Hydjet
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/v7/tt_230127_pixel_230126_Hydjet_Drum5F_PbPb_5360GeV_221224_GTv7priZ0_GTv8Th4.root,Hydjet-v7
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230130_bkg5_pixel_230129_Hydjet_Drum5F_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,Hydjet-bkg5
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_Hydjet_Drum5F_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,Hydjet
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_EposLHC_ReggeGribovParton_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,Epos
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_AMPT_StringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,AMPTstring
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_AMPT_NoStringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,AMPTnostring
)

INPUTS_DATA=(
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230127_pixel_230126_HITestRaw0-5_HIRun2022A_MBPVfilTh4_362294.root,362294t
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221218_pixelpre_221201_HITestRaw0-6_HIRun2022A_MBPVfilTh4_362294.root,362294t
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_EposLHC_ReggeGribovParton_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,EposClose
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_AMPT_StringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,AMPTstringClose
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_AMPT_NoStringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,AMPTnostringClose
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_Hydjet_Drum5F_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,HydjetClose
)

for mm in ${INPUTS_MC[@]}
do
    IFS=',' ; VINPUTS_MC=($mm) ; unset IFS ;
    INPUT_MC=${VINPUTS_MC[0]}
    TAG_MC=${VINPUTS_MC[1]}

    echo $INPUT_MC
    echo $TAG_MC

    # correction
    [[ ${1:-0} -eq 1 ]] && {
        for t in ${TYPES[@]}
        do
            ./reap_results $t $INPUT_MC ${TAG_MC}_${tag} 0 20 0 0 0 1 "null" 0 $maxdr2 ${tag} "null" 0 "(1)" &
        done
        wait
    }

    for dd in ${INPUTS_DATA[@]}
    do
        IFS=',' ; VINPUTS_DATA=($dd) ; unset IFS ;
        INPUT_DATA=${VINPUTS_DATA[0]}
        TAG_DATA=${VINPUTS_DATA[1]}
        ESTAG=${TAG_DATA%%Close}
        [[ $ESTAG == $TAG_DATA ]] && ESTAG=$TAG_MC # estag for real data?

        echo " "$INPUT_DATA
        echo " "$TAG_DATA
        echo " "$ESTAG

        # apply
        [[ ${2:-0} -eq 1 ]] && {
            for t in ${TYPES[@]}
            do
                # ./reap_results $t $INPUT_DATA ${TAG_DATA}-${TAG_MC}_${tag} 0 20 ${TAG_MC}_${tag} 1 0 1 ${ESTAG}_${tag} 0 $maxdr2 ${TAG_DATA}-${TAG_MC}_${tag} "null" 0 "(1)" $nentries &
                ./reap_results $t $INPUT_DATA ${TAG_DATA}-${TAG_MC}_${tag} 0 20 ${TAG_MC}_${tag} 1 1 1 ${ESTAG}_${tag} 0 $maxdr2 ${TAG_DATA}-Hydjet_${tag} "null" 0 "(1)" $nentries &
                # ./reap_results $t $INPUT_DATA ${TAG_DATA}-${TAG_MC}_${tag} 0 20 ${TAG_MC}_${tag} 1 1 1 ${ESTAG}_${tag} 0 $maxdr2 ${TAG_DATA}-${TAG_MC}_${tag} "null" 0 "(1)" $nentries &
                # ./reap_results $t $INPUT_DATA ${TAG_DATA}-${TAG_MC}_${tag} 0 20 ${TAG_MC}_${tag} 1 1 1 ${TAG_MC}_${tag} 0 $maxdr2 ${TAG_DATA}-${TAG_MC}_${tag} "null" 0 "(1)" $nentries &
            done
            wait
        }
    done
done

