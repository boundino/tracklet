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


tagverdefault="v3"
#
TYPES=(12 13 14 23 24 34 56 57 67)
CENTS=(4 20)
# for i in {20..5} ; do CENTS+=($((i-1)) $i) ; done ;

##
INPUTS_MC=/eos/cms/store/cmst3/user/wangj/tracklet/tt_230724_pixel_230724_EposLHC_ReggeGribovParton_5360GeV_1255p1.root,epos
INPUTS_DATA=/eos/cms/store/cmst3/user/wangj/tracklet/tt_230724_pixel_230724_HITestRaw0-6_HIRun2022A_MBPVfilTh4_362294.root,362294

source tool.shinc 

#

IFS=',' ; VINPUTS_MC=($INPUTS_MC) ; unset IFS ;
INPUT_MC=${VINPUTS_MC[0]}
TAG_MC=${VINPUTS_MC[1]}

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
                       $ctable "$asel" \
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
    tages="epos.m."$tagverdefault".s."$cmin"."$cmax ;
    [[ $TAG_MC == *epos* ]] && {
        # ==> tag name
        [[ ${label:1:1} -eq 1 ]] && tages=$TAG_MC"."$tcgm"."$tagver".s."$cmin"."$cmax
        echo '$tages : '$tages
        # <== tag name
        [[ ${execu:1:1} -eq 1 ]] && {
            for t in ${TYPES[@]} ; do
                set -x
                ./reap_results $t $INPUT_MC $tages $cmin $cmax \
                               0 ${cgm:0:1} ${cgm:1:1} ${cgm:2:1} "null" \
                               $multhandle $maxdr2 0 "null" \
                               3 "$asel" \
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
    }

    ##############################################
    #      apply correction, w. centrality       #
    ##############################################

    tcgm=cgm # correction, geometric, acceptance map
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
        truth="epos.m.v3.s."$cmin"."$cmax"&"${taglabel[epos]}"&2,hydjet.m.v3.s."$cmin"."$cmax"&"${taglabel[hydjet]}"&1,amptsm.m.v3.s."$cmin"."$cmax"&"${taglabel[amptsm]}"&4,amptnm.m.v3.s."$cmin"."$cmax"&"${taglabel[amptnm]}"&6"
        ./merge_monde $tagappl "${taglabel[${TAG_DATA%%CLOSE}]} corr. w. ${taglabel[$TAG_MC]}" $mergecomb "$truth"
    }
    
    c=$((c+2))

done # while [ $c -lt $((${#CENTS[@]}-1)) ]


