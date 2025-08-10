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


tagverdefault="v1"
mcdefault=hijing
#
TYPES=(12 13 14 23 24 34 56 57 67)
CENTS=(0 20)
for i in {20..4} ; do CENTS+=($((i-1)) $i) ; done ;

##
INPUTS_MC=/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250727_weight_pixel_250727_Hijing_OO_5362GeV_pf_realistic_mask.root,hijing,7
INPUTS_DATA=/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250727_pixel_250727_IonPhysics0_394153_mask_p10.root,394153,0

source tool.shinc 

#

IFS=',' ; VINPUTS_MC=($INPUTS_MC) ; unset IFS ;
INPUT_MC=${VINPUTS_MC[0]}
TAG_MC=${VINPUTS_MC[1]}
CENT_MC=${VINPUTS_MC[2]}

IFS=',' ; VINPUTS_DATA=($INPUTS_DATA) ; unset IFS ;
INPUT_DATA=${VINPUTS_DATA[0]}
TAG_DATA=${VINPUTS_DATA[1]}

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
                       $CENT_MC "$asel" \
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
    input_cent_corr=$INPUT_MC
    input_cent=$ctable
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
                           $input_cent "$asel" \
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

    tcgm=cgm # correction, geometric, acceptance map # !!!!! should be cgm
    cgm=$(getcgm $tcgm)

    # ==> tag name
    tagappl=$TAG_DATA"."$tcgm"."$TAG_MC".m."$tagver".s."$cmin"."$cmax
    echo '  $tagappl : '$tagappl
    echo '  $tages : '$tages
    [[ ${execu:2:1} -eq 1 ]] && {
        # <== tag name
        for t in ${TYPES[@]} ; do
            set -x
            ./reap_results $t $INPUT_DATA $tagappl $cmin $cmax \
                           $tagcorr ${cgm:0:1} ${cgm:1:1} ${cgm:2:1} $tages \
                           $multhandle $maxdr2 $tagdr "null" \
                           $ctable "$asel" \
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
        truth="hijing.m.v1.s."$cmin"."$cmax"&"${taglabel[hijing]}"&2","amptnm.m.v0.s."$cmin"."$cmax"&"${taglabel[amptnm]}"&6","amptsm.m.v0.s."$cmin"."$cmax"&"${taglabel[amptsm]}"&4","hydjet.m.v0.s."$cmin"."$cmax"&"${taglabel[hydjet]}"&9","angantyr.m.v0.s."$cmin"."$cmax"&"${taglabel[angantyr]}"&8","epos.m.v0.s."$cmin"."$cmax"&"${taglabel[epos]}"&10"
        ./merge_monde $tagappl "$(ftaglabel ${TAG_DATA%%CLOSE}) corr. w. ${taglabel[$TAG_MC]}" $mergecomb "$truth"
    }
    
    c=$((c+2))

done # while [ $c -lt $((${#CENTS[@]}-1)) ]


