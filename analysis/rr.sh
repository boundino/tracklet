#!/bin/bash

make reap_results || exit 1

maxdr2=0.25 ; tagdr="drlt0p5" ; tagver="v1"
TYPES=(12 13 14 23 24 34 56 57 67)
# TYPES=(11 22 33 44 55 66 77)
# CENTS=(20 18 16 14 12 10 8 6)
CENTS=(20 4)
##
INPUTS_MC=(
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230322_pixel_230129_EposLHC_ReggeGribovParton_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,epos
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230130_pixel_230129_EposLHC_ReggeGribovParton_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,epos
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230315_samelayer_pixel_230129_EposLHC_ReggeGribovParton_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,epos
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230130_pixel_230129_Hydjet_Drum5F_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,hydjet
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230130_pixel_230129_AMPT_StringMelting_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,amptsm
)

INPUTS_DATA=(
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230322_pixel_230126_HITestRaw0-6_HIRun2022A_MBPVfilTh4_362294.root,362294
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230127_pixel_230126_HITestRaw0-5_HIRun2022A_MBPVfilTh4_362294.root,362294
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230315_samelayer_pixel_230126_HITestRaw0-6_HIRun2022A_MBPVfilTh4_362294.root,362294
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230130_pixel_230129_EposLHC_ReggeGribovParton_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,eposClose
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230130_pixel_230129_Hydjet_Drum5F_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,hydjetClose
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230130_pixel_230129_AMPT_StringMelting_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,amptsmClose
)

function getcgm()
{
    options=("c" "g" "m")
    for char in ${options[@]}
    do
        [[ $1 == *"$char"* ]] && echo -n 1 || echo -n 0
    done
}

multhandle=0
ctable=0
for mm in ${INPUTS_MC[@]}
do
    IFS=',' ; VINPUTS_MC=($mm) ; unset IFS ;
    INPUT_MC=${VINPUTS_MC[0]}
    TAG_MC=${VINPUTS_MC[1]}

    echo $INPUT_MC
    echo $TAG_MC

    # correction
    tcgm=m # correction, geometric, acceptance map
    cgm=$(getcgm $tcgm)
    # ==> tag name
    tagcorr="incl."$TAG_MC"."$tcgm"."$tagver
    echo $tagcorr
    # <== tag name
    for t in ${TYPES[@]}
    do
        [[ ${1:-0} -eq 1 ]] && {
            set -x
            ./reap_results $t $INPUT_MC $tagcorr 0 20 \
                           0 ${cgm:0:1} ${cgm:1:1} ${cgm:2:1} "null" \
                           $multhandle $maxdr2 0 "null" \
                           $ctable "(1)" &
            set +x
        }
    done
    wait
    
    for dd in ${INPUTS_DATA[@]}
    do
        IFS=',' ; VINPUTS_DATA=($dd) ; unset IFS ;
        INPUT_DATA=${VINPUTS_DATA[0]}
        TAG_DATA=${VINPUTS_DATA[1]}
        # tages=${TAG_DATA%%Close}
        # [[ $tages == $TAG_DATA ]] && tages=$TAG_MC # estag for real data?

        echo " "$INPUT_DATA
        echo " "$TAG_DATA
        
        # apply
        tcgm=cgm # correction, geometric, acceptance map
        [[ $TAG_DATA == *Close ]] && {
            tcgm=cm ; tages=$tagcorr ; } || {
            tages="incl.epos.m.v1" ; }
        cgm=$(getcgm $tcgm)
        c=0
        while [ $c -lt $((${#CENTS[@]}-1)) ]
        do
            cmin=${CENTS[c+1]}
            cmax=${CENTS[c]}

            # ==> tag name
            tagappl=$TAG_DATA"."$tcgm"."${tagcorr##incl.}".s."$cmin"."$cmax
            echo $tagappl
            # <== tag name
            for t in ${TYPES[@]}
            do
                [[ ${2:-0} -eq 1 ]] && {
                    ./reap_results $t $INPUT_DATA $tagappl $cmin $cmax \
                                   $tagcorr ${cgm:0:1} ${cgm:1:1} ${cgm:2:1} $tages \
                                   $multhandle $maxdr2 $tagdr "null" \
                                   $ctable "(1)" &
                    # ./reap_results $t $INPUT_DATA ${TAG_DATA}-${TAG_MC}_${tag} 0 20 ${TAG_MC}_${tag} 1 0 1 ${ESTAG}_${tag} 0 $maxdr2 ${TAG_DATA}-${TAG_MC}_${tag} "null" 0 "(1)" $nentries &
                    # ./reap_results $t $INPUT_DATA ${TAG_DATA}-${TAG_MC}_${tag} 0 20 ${TAG_MC}_${tag} 1 1 1 ${ESTAG}_${tag} 0 $maxdr2 ${TAG_DATA}-Hydjet_${tag} "null" 0 "(1)" $nentries &
                    # ./reap_results $t $INPUT_DATA ${TAG_DATA}-${TAG_MC}_${tag} 0 20 ${TAG_MC}_${tag} 1 1 1 ${ESTAG}_${tag} 0 $maxdr2 ${TAG_DATA}-${TAG_MC}_${tag} "null" 0 "(1)" $nentries &
                    # ./reap_results $t $INPUT_DATA ${TAG_DATA}-${TAG_MC}_${tag} 0 20 ${TAG_MC}_${tag} 1 1 1 ${TAG_MC}_${tag} 0 $maxdr2 ${TAG_DATA}-${TAG_MC}_${tag} "null" 0 "(1)" $nentries &
                }
            done
            # wait
            c=$((c+1))
        done

        # [[ ${4:-0} -eq 1 ]] && {
        #     for t in ${TYPES[@]}
        #     do
        #         for c in ${CENTS[@]}
        #         do
        #             cmin=$((c-1))
        #             cmax=$c
        #             echo ${TAG_DATA}-${TAG_MC}_${tag}".a."$cmin"."$cmax
        #             ./reap_results $t $INPUT_DATA ${TAG_DATA}-${TAG_MC}_${tag}".a."$cmin"."$cmax $cmin $cmax ${TAG_MC}_${tag} 1 1 1 ${ESTAG}_${tag} 0 $maxdr2 ${TAG_DATA}-Hydjet_${tag} "null" 0 &
        #         done
        #         wait
        #     done
        # } # [[ ${4:-0} -eq 1 ]] &&
    done
done

