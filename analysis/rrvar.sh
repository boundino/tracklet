#!/bin/bash

# ./rrvar.sh [execu] [tagver] [asel] [maxdr] [tagdr] [multhandle] [ctable]
execu=${1:-0000}
label=${2:-00}
tagver=${3:-"v2"}
asel=${4:-"(1)"}
maxdr2=${5:-0.25}
tagdr=${6:-"drlt0p5"}
multhandle=${7:-0}
ctable=${8:-0}
hfsel=${9:-"(nhfn>1 && nhfp>1)"}
fullstats=${10:-0}

tagverdefault="v1"
mcdefault=epos
#
TYPES=(12 13 14 23 24 34 56 57 67)
CENTS=(0 20)
for i in {20..7} ; do CENTS+=($((i-1)) $i) ; done ;
CENTS+=(18 20)

##
INPUTS_MC=/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250810_weight_pixel_250810_MINI_0731_Hijing_OO_5362GeV_pf_realistic_maskf.root,hijing,7
INPUTS_DATA=/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250810_pixel_250810_IonPhysics0_394153_mask_p10.root,394153,0
INPUT_DATA_FULL=/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250810_pixel_250810_IonPhysics0_394153_mask.root
INPUTS_ES=/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250810_weight_pixel_250809_MINI_0731_EposLHC_ReggeGribovParton_OO_5362GeV_pf_realistic_maskf.root,epos,3

source tool.shinc 

#

IFS=',' ; VINPUTS_MC=($INPUTS_MC) ; unset IFS ;
INPUT_MC=${VINPUTS_MC[0]}
TAG_MC=${VINPUTS_MC[1]}
CENT_MC=${VINPUTS_MC[2]}

IFS=',' ; VINPUTS_DATA=($INPUTS_DATA) ; unset IFS ;
INPUT_DATA=${VINPUTS_DATA[0]}
TAG_DATA=${VINPUTS_DATA[1]}

IFS=',' ; VINPUTS_ES=($INPUTS_ES) ; unset IFS ;
INPUT_ES=${VINPUTS_ES[0]}
TAG_ES=${VINPUTS_ES[1]}
CENT_ES=${VINPUTS_ES[2]}

##############################################
#  create correction, no centrality -> incl  #
##############################################

tcgm=m # correction, geometric, acceptance map
cgm=$(getcgm $tcgm)
# ==> tag name
tagcorr="incl."$TAG_MC"."$tcgm"."$tagverdefault
[[ ${label:0:1} -eq 1 ]] && tagcorr="incl."$TAG_MC"."$tcgm"."$tagver
echo '$tagcorr : '$tagcorr
# <== tag name
[[ ${execu:0:1} -eq 1 ]] && {
    for t in ${TYPES[@]}
    do
        set -x
        ./reap_results $t $INPUT_MC $tagcorr 0 20 \
                       0 ${cgm:0:1} ${cgm:1:1} ${cgm:2:1} "null" \
                       $multhandle $maxdr2 0 "null" \
                       $CENT_MC "$asel" "$hfsel" \
                       2>&1 | tee logs/$tagcorr-$t.txt & # \
            set +x
    done # for t in ${TYPES[@]}
    wait
}
# trash figs/corrections/empty-$tagcorr-*.png 

c=0
while [ $c -lt $((${#CENTS[@]}-1)) ] ; do
    cmin=${CENTS[c]}
    cmax=${CENTS[c+1]}

    ##############################################
    #  create correction empty, w. centrality    #
    ##############################################

    tcgm=m # correction, geometric, acceptance map
    cgm=$(getcgm $tcgm)
    tagveres=$tagverdefault
    [[ ${label:1:1} -eq 1 ]] && { tagveres=$tagver ; }
    tages=$mcdefault".m."$tagveres".s."$cmin"."$cmax ;
    input_cent_corr=$INPUT_ES
    input_cent=$CENT_ES
    [[ $TAG_DATA == *CLOSE ]] && {
        tages=${TAG_DATA/CLOSE/}"."$tcgm"."$tagveres".s."$cmin"."$cmax            
        input_cent_corr=$INPUT_DATA
        input_cent=$ctable
    }
    echo '$tages : '$tages
    # <== tag name
    [[ ${execu:1:1} -eq 1 ]] && {
        
        for t in ${TYPES[@]} ; do
            set -x
            ./reap_results $t $input_cent_corr $tages $cmin $cmax \
                           0 ${cgm:0:1} ${cgm:1:1} ${cgm:2:1} "null" \
                           $multhandle $maxdr2 0 "null" \
                           $input_cent "$asel" "$hfsel" \
                           2>&1 | tee logs/$tages-$t.txt & # \
                set +x
        done # for t in ${TYPES[@]}
        wait
        trash figs/corrections/alpha-$tages-*.png
        [[ $cmin -eq 0 && $cmax -eq 20 ]] || {
            trash figs/corrections/sdfrac-$tages-*.png \
                  figs/corrections/trigger-$tages-*.png \
                  figs/acceptance/accep-$tages-*.png \
                  figspdf/fits/alphafit-$tages-*.pdf
        }
    }

    ##############################################
    #      apply correction, w. centrality       #
    ##############################################

    tcgm=cgm # correction, geometric, acceptance map 
    cgm=$(getcgm $tcgm)

    input_data_sample=$INPUT_DATA
    [[ $fullstats -gt 0 && $cmin -eq 0 && $cmax -eq 20 ]] && {
        input_data_sample=$INPUT_DATA_FULL
    }
    # ==> tag name
    tagappl=$TAG_DATA"."$tcgm"."$TAG_MC".m."$tagver".s."$cmin"."$cmax
    echo '  $tagappl : '$tagappl
    echo '  $tages : '$tages
    [[ ${execu:2:1} -eq 1 ]] && {
        # <== tag name
        for t in ${TYPES[@]} ; do
            set -x
            ./reap_results $t $input_data_sample $tagappl $cmin $cmax \
                           $tagcorr ${cgm:0:1} ${cgm:1:1} ${cgm:2:1} $tages \
                           $multhandle $maxdr2 $tagdr "null" \
                           $ctable "$asel" "$hfsel" \
                           2>&1 | tee logs/$tagappl-$t.txt & # \
                set +x
        done # for t in ${TYPES[@]}
        wait
    }

    ##############################################
    #    average combinations, w. centrality     #
    ##############################################
    [[ ${execu:3:1} -eq 1 ]] && {
        mergecomb=
        for t in ${TYPES[@]} ; do
	    mergecomb=$mergecomb","$t
        done
        mergecomb=${mergecomb##,}
        truth="hijing.m.v1.s."$cmin"."$cmax"&"${taglabel[hijing]}"&2","amptnm.m.v1.s."$cmin"."$cmax"&"${taglabel[amptnm]}"&6","amptsm.m.v1.s."$cmin"."$cmax"&"${taglabel[amptsm]}"&4","hydjet.m.v1.s."$cmin"."$cmax"&"${taglabel[hydjet]}"&9","angantyr.m.v1.s."$cmin"."$cmax"&"${taglabel[angantyr]}"&8","epos.m.v1.s."$cmin"."$cmax"&"${taglabel[epos]}"&10"
        ./merge_monde $tagappl "$(ftaglabel ${TAG_DATA%%CLOSE}) corr. w. ${taglabel[$TAG_MC]}" $mergecomb "$truth"
    }
    
    c=$((c+2))

done # while [ $c -lt $((${#CENTS[@]}-1)) ]
