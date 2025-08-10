#!/bin/bash

make comp_avg || exit 1
make sum_systematics || exit 1

source tool.shinc 

group="394153.cgm.hijing.m.v1"
mcdefault=hijing

CENTS=(0 20)
for i in {20..4} ; do CENTS+=($((i-1)) $i) ; done ;

##
c=0
while [ $c -lt $((${#CENTS[@]}-1)) ] ; do
    cmin=${CENTS[c]}
    cmax=${CENTS[c+1]}

    CONFIGS=(
        "${group}.s.$cmin.$cmax&#Deltar < 0.5","${group}-drlt0p4.s.$cmin.$cmax&#Deltar < 0.4","${group}-drlt0p6.s.$cmin.$cmax&#Deltar < 0.6"%"dr-${group}.s.$cmin.$cmax"%"394153 corr. w. ${taglabel[$mcdefault]}"%"1"%"tracklet #Deltar selection"
        "${group}.s.$cmin.$cmax&-15 < v_{z} < 15","${group}-vz1.s.$cmin.$cmax&-10 < v_{z} < -5","${group}-vz2.s.$cmin.$cmax&-5 < v_{z} < 0","${group}-vz3.s.$cmin.$cmax&0 < v_{z} < 5","${group}-vz4.s.$cmin.$cmax&5 < v_{z} < 10"%"vz-${group}.s.$cmin.$cmax"%"394153 corr. w. ${taglabel[$mcdefault]}"%"1"%"event v_{z}"
        # "${group}.s.$cmin.$cmax&${taglabel[$mcdefault]}","${group/$mcdefault/hydjet}.s.$cmin.$cmax&${taglabel[hydjet]}","${group/$mcdefault/amptnm}.s.$cmin.$cmax&${taglabel[amptnm]}","${group/$mcdefault/amptnm2}.s.$cmin.$cmax&394153""%""mc-${group}.s.$cmin.$cmax"%"394153 corr. w. different MC"%"1"%"model dependence"
        "${group}.s.$cmin.$cmax&number of tracklets","${group}-nhit2.s.$cmin.$cmax&number of hits"%"mult-${group}.s.$cmin.$cmax"%"394153 corr. w. ${taglabel[$mcdefault]}"%"1"%"correction factorization"
        # "${group}.s.$cmin.$cmax&nominal","${group}-split.s.$cmin.$cmax&pixel splitting"%"split-${group}.s.$cmin.$cmax"%"394153 corr. w. ${taglabel[$mcdefault]}"%"1"%"pixel splitting"
        "${group}.s.$cmin.$cmax&nominal","${group}-hfeffup.s.$cmin.$cmax&centrality up","${group}-hfeffdown.s.$cmin.$cmax&centrality down"%"ctable-${group}.s.$cmin.$cmax"%"394153 corr. w. ${taglabel[$mcdefault]}"%"1"%"centrality table"
    )
    [[ $cmin -eq 0 && $cmax -eq 20 ]] && {
        CONFIGS+=(
            "${group}.s.$cmin.$cmax&tracklet count.","${group/cgm/cm}-clus.s.$cmin.$cmax&cluster count."%"method-${group}.s.$cmin.$cmax"%"394153 corr. w. ${taglabel[${mcdefault}]}"%"0"%"-"
            # "${group}.s.$cmin.$cmax&${taglabel[amptnm]}","${group/${taglabel[amptnm]}/362318}.s.$cmin.$cmax&362318"%"run-cgm.${mcdefault}.m.v3.s.$cmin.$cmax"%"Different run corr. w. ${taglabel[${mcdefault}]}"%"0"%"-"
            "${group}.s.$cmin.$cmax&all lumi","${group}-lumi0.s.$cmin.$cmax&52 <= lumi < 100","${group}-lumi1.s.$cmin.$cmax&100 <= lumi < 164"%"lumi-${group}.s.$cmin.$cmax"%"394153 corr. w. ${taglabel[${mcdefault}]}"%"0"%"-"            
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

        ./sum_systematics "$systs" "$group.s.$cmin.$cmax" "394153 corr. w. ${taglabel[${mcdefault}]}"
    }
    echo

    c=$((c+2))
done
