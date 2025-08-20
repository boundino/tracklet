#!/bin/bash

make comp_avg || exit 1
make sum_systematics || exit 1

source tool.shinc 

mcdefault=hijing
group="394153.cgm.hijing.m.v1"

CENTS=(0 20)
for i in {20..7} ; do CENTS+=($((i-1)) $i) ; done ;
# CENTS+=(18 20)

##
c=0
# 000001111111111111111111111111100000
while [ $c -lt $((${#CENTS[@]}-1)) ] ; do
    cmin=${CENTS[c]}
    cmax=${CENTS[c+1]}
    echo "centrality: "$cmin" - "$cmax

    CONFIGS=(
        "${group}.s.$cmin.$cmax&-15 < v_{z} < 15","${group}-vz1.s.$cmin.$cmax&-10 < v_{z} < -5&000000011111111111111111111111100000","${group}-vz2.s.$cmin.$cmax&-5 < v_{z} < 0&000001111111111111111111111110000000","${group}-vz3.s.$cmin.$cmax&0 < v_{z} < 5&000000111111111111111111111100000000","${group}-vz4.s.$cmin.$cmax&5 < v_{z} < 10&000001111111111111111111111000000000"%"vz-${group}.s.$cmin.$cmax"%"394153 corr. w. ${taglabel[$mcdefault]}"%"1"%"event v_{z}"
        "${group}.s.$cmin.$cmax&#Deltar < 0.5","${group}-drlt0p4.s.$cmin.$cmax&#Deltar < 0.4","${group}-drlt0p6.s.$cmin.$cmax&#Deltar < 0.6"%"dr-${group}.s.$cmin.$cmax"%"394153 corr. w. ${taglabel[$mcdefault]}"%"1"%"tracklet #Deltar selection"
        "${group}.s.$cmin.$cmax&${taglabel[$mcdefault]}","${group/$mcdefault/amptnm}.s.$cmin.$cmax&$(ftaglabel amptnm)","${group/$mcdefault/amptsm}.s.$cmin.$cmax&$(ftaglabel amptsm)","${group/$mcdefault/epos}.s.$cmin.$cmax&$(ftaglabel epos)"%"mc-${group}.s.$cmin.$cmax"%"394153 corr. w. different MC"%"1"%"model dependence"
        "${group}.s.$cmin.$cmax&number of tracklets","${group}-nhit2.s.$cmin.$cmax&number of hits"%"mult-${group}.s.$cmin.$cmax"%"394153 corr. w. ${taglabel[$mcdefault]}"%"1"%"correction factorization"
        "${group}.s.$cmin.$cmax&nominal","${group}-hfeffup.s.$cmin.$cmax&centrality up","${group}-hfeffdown.s.$cmin.$cmax&centrality down"%"ctable-${group}.s.$cmin.$cmax"%"394153 corr. w. ${taglabel[$mcdefault]}"%"1"%"centrality table"
        "${group}.s.$cmin.$cmax&$(ftaglabel epos)","${group}-sdf1.s.$cmin.$cmax&$(ftaglabel angantyr)"%"sdf-${group}.s.$cmin.$cmax"%"Single diffractive correction"%"1"%"single diffraction fraction"
        "${group}.s.$cmin.$cmax&HFCoin2Th4","${group}-evtsel1.s.$cmin.$cmax&HFCoinc3Th3","${group}-evtsel2.s.$cmin.$cmax&HFCoinc2Th5"%"evtsel-${group}.s.$cmin.$cmax"%"394153 corr. w. ${taglabel[$mcdefault]}"%"1"%"event selection"
        # # "${group}.s.$cmin.$cmax&nominal","${group}-split.s.$cmin.$cmax&pixel splitting"%"split-${group}.s.$cmin.$cmax"%"394153 corr. w. ${taglabel[$mcdefault]}"%"1"%"pixel splitting"
    )
    [[ $cmin -eq 0 && $cmax -eq 20 ]] && {
        CONFIGS+=(
            "${group}.s.$cmin.$cmax&tracklet count.","${group/cgm/cm}-clus.s.$cmin.$cmax&cluster count."%"method-${group}.s.$cmin.$cmax"%"394153 corr. w. ${taglabel[${mcdefault}]}"%"0"%"-"
            "${group}.s.$cmin.$cmax&all lumi","${group}-lumi2.s.$cmin.$cmax&52 <= lumi < 75","${group}-lumi3.s.$cmin.$cmax&75 <= lumi < 100","${group}-lumi4.s.$cmin.$cmax&100 <= lumi < 130","${group}-lumi5.s.$cmin.$cmax&130 <= lumi < 164"%"lumi-${group}.s.$cmin.$cmax"%"394153 corr. w. ${taglabel[${mcdefault}]}"%"0"%"-"            
            "${group}.s.$cmin.$cmax&${taglabel[$mcdefault]}","${group/$mcdefault/amptnm}.s.$cmin.$cmax&$(ftaglabel amptnm)","${group/$mcdefault/amptsm}.s.$cmin.$cmax&$(ftaglabel amptsm)","${group/$mcdefault/epos}.s.$cmin.$cmax&$(ftaglabel epos)","${group/$mcdefault/hydjet}.s.$cmin.$cmax&$(ftaglabel hydjet)"%"mcall-${group}.s.$cmin.$cmax"%"394153 corr. w. different MC"%"0"%"-"
            # # "${group}.s.$cmin.$cmax&$(ftaglabel amptnm)","${group/$(ftaglabel amptnm)/362318}.s.$cmin.$cmax&362318"%"run-cgm.${mcdefault}.m.v3.s.$cmin.$cmax"%"Different run corr. w. ${taglabel[${mcdefault}]}"%"0"%"-"
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
