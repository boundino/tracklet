#!/bin/bash

make comp_avg || exit 1
make sum_systematics || exit 1

source tool.shinc 

group="362294.cgm.epos.m.v1"

CENTS=(4 20)
# for i in {20..5} ; do CENTS+=($((i-1)) $i) ; done ;
# for i in 20 10 ; do CENTS+=($((i-1)) $i) ; done ;

##
c=0
while [ $c -lt $((${#CENTS[@]}-1)) ] ; do
    cmin=${CENTS[c]}
    cmax=${CENTS[c+1]}

    CONFIGS=(
        "${group}.s.$cmin.$cmax&#Deltar < 0.5,${group}-drlt0p4.s.$cmin.$cmax&#Deltar < 0.4,${group}-drlt0p6.s.$cmin.$cmax&#Deltar < 0.6%dr-${group}.s.$cmin.$cmax%362294 corr. w. ${taglabel[epos]}%1%tracklet #Deltar selection"
        "${group}.s.$cmin.$cmax&-15 < v_{z} < 15,${group}-vz1.s.$cmin.$cmax&-10 < v_{z} < -5,${group}-vz2.s.$cmin.$cmax&-5 < v_{z} < 0,${group}-vz3.s.$cmin.$cmax&0 < v_{z} < 5,${group}-vz4.s.$cmin.$cmax&5 < v_{z} < 10%vz-${group}.s.$cmin.$cmax%362294 corr. w. ${taglabel[epos]}%1%event v_{z}"
        "${group}.s.$cmin.$cmax&${taglabel[epos]},${group/epos/hydjet}.s.$cmin.$cmax&${taglabel[hydjet]},${group/epos/amptsm}.s.$cmin.$cmax&${taglabel[amptsm]}%mc-${group/.epos/}.s.$cmin.$cmax%362294 corr. w. different MC%1%model dependence"
        # "${group}.s.$cmin.$cmax&nominal,${group}-split.s.$cmin.$cmax&pixel splitting%split-${group}.s.$cmin.$cmax%362294 corr. w. ${taglabel[epos]}%1%pixel splitting"
        "${group}.s.4.20&nominal,${group}-split.s.4.20&pixel splitting%split-${group}.s.$cmin.$cmax%362294 corr. w. ${taglabel[epos]}%1%pixel splitting"
    )
    [[ $cmin -eq 4 && $cmax -eq 20 ]] && {
        CONFIGS+=(
            "${group}.s.$cmin.$cmax&tracklet count.,362294.cm.epos.m.v1.s.$cmin.$cmax&pixel count.%method-${group}.s.$cmin.$cmax%362294 corr. w. ${taglabel[epos]}%0%-"
            "${group}.s.$cmin.$cmax&362294,${group/362294/362318}.s.$cmin.$cmax&362318%run-cgm.epos.m.v1.s.$cmin.$cmax%Different run corr. w. ${taglabel[epos]}%0%-"
            "${group}.s.$cmin.$cmax&GTv8 + Private BS,362294.cgm.epos.m.v2.s.$cmin.$cmax&GTv9%gt-cgm.epos.m.v1.s.$cmin.$cmax%362294 corr. w. ${taglabel[epos]}%0%-"
        )
    }

    systs=""
    #
    for mm in "${CONFIGS[@]}"
    do
        IFS='%' ; VCONFIG=($mm) ; unset IFS ;
        input=${VCONFIG[0]}
        tag=${VCONFIG[1]}
        text=${VCONFIG[2]}
        insyst=${VCONFIG[3]}
        systleg=${VCONFIG[4]}

        [[ $insyst -eq 1 ]] && {
            echo -en "\e[1m[systematics]\e[0m"
            systs=$systs",results/comp-"$tag"&"$systleg
        } || echo -en "\e[2m[cross-check]\e[0m"
        echo -e " \e[4m$tag\e[0m"

        [[ ${1:-0} -eq 1 ]] && {
            echo $text
            checkfiles "$input" "output/avg-"
            
            ./comp_avg "$input" $tag "$text"
        }
    done # for mm in ${CONFIGS[@]}
    systs=${systs##,}
    systs="output/avg-$group.s.$cmin.$cmax&deviation from average,"$systs

    [[ ${2:-0} -eq 1 ]] && {
        echo -e "\e[7m[sum systematics]\e[0m"
        checkfiles "$systs"

        ./sum_systematics "$systs" "$group.s.$cmin.$cmax" "362294 corr. w. ${taglabel[epos]}"
    }
    echo

    c=$((c+2))
done
