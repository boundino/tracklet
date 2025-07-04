#!/bin/bash

make reap_results || exit 1
make merge_monde || exit 1

maxdr2=0.25 ; tagdr="drlt0p5" 
tagver="v0" ; nominal="hijing" ; corrtagver="v0"

TYPES=(12 13 14 23 24 34 56 57 67)
# TYPES=(11 22 33 44 55 66 77) ; tagver=$tagver"-clus" ; corrtagver=$corrtagver"-clus" ;
CENTS=(0 20) # (4 20)
for i in {20..1} ; do CENTS+=($((i-1)) $i) ; done ; 

##
INPUTS_MC=(
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_Hijing_OO_5360GeV_0527_v2.root,hijing,7
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_AMPT_NoStringMelting_OO_5360GeV_0527_v2.root,amptnm,5
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_AMPT_NoStringMeltingp0_OO_5360GeV_0527_v2.root,amptnm2,8 # !
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_Hydjet_Drum5F_OO_5360GeV_0527_v2.root,hydjet,4
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_AMPT_StringMelting_OO_5360GeV_0527_v2.root,amptsm,6
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_Pythia_Angantyr_OO_5360GeV_0527_v2.root,angantyr,0

    # clus
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_Hijing_OO_5360GeV_0527_v2_clus.root,hijing,7
    
    # small
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_Hijing_NoPU_100kEvents_OO_5360GeV_GenSim_030825.root,hijings,7
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_AMPT_NoPU_100kEvents_OO_5360GeV_GenSim_032525.root,amptnm2s,8
)

INPUTS_DATA=(
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_Hijing_OO_5360GeV_0527_v2.root,hijingCLOSE,7
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_AMPT_NoStringMelting_OO_5360GeV_0527_v2.root,amptnmCLOSE,5
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_AMPT_NoStringMeltingp0_OO_5360GeV_0527_v2.root,amptnm2CLOSE,8 # ! 
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_AMPT_StringMelting_OO_5360GeV_0527_v2.root,amptsmCLOSE,6 
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_Hydjet_Drum5F_OO_5360GeV_0527_v2.root,hydjet,4
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_AMPT_NoPU_100kEvents_OO_5360GeV_GenSim_032525.root,amptnmCLOSE,5

    # clus
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_AMPT_NoStringMeltingp0_OO_5360GeV_0527_v2_clus.root,amptnm2CLOSE,8 # ! 
    
    # small
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_AMPT_NoPU_100kEvents_OO_5360GeV_GenSim_032525.root,amptnm2sCLOSE,8
    # 2022
    # /eos/cms/store/cmst3/user/wangj/tracklet/tt_230724_pixel_230724_HITestRaw0-6_HIRun2022A_MBPVfilTh4_362294.root,362294
    # /eos/cms/store/cmst3/user/wangj/tracklet/tt_230724_wclus_pixel_230724_HITestRaw0-5_HIRun2022A_MBPVfilTh4_362294.root,362294
    # /eos/cms/store/cmst3/user/wangj/tracklet/tt_230724_pixel_230724_HITestRaw0-1_HIRun2022A_MBPVfilTh4_362318.root,362318
)

source tool.shinc 

multhandle=0
ctable=0
mkdir -p logs

#
for mm in ${INPUTS_MC[@]}
do
    IFS=',' ; VINPUTS_MC=($mm) ; unset IFS ;
    INPUT_MC=${VINPUTS_MC[0]}
    TAG_MC=${VINPUTS_MC[1]}
    CENT_MC=${VINPUTS_MC[2]}

    echo $INPUT_MC
    echo $TAG_MC

    ##############################################
    #  create correction, no centrality -> incl  #
    ##############################################

    tcgm=m # correction, geometric, acceptance map
    cgm=$(getcgm $tcgm)
    # ==> tag name
    # tagcorr="incl."$TAG_MC"."$tcgm"."$tagver
    tagcorr="incl."$TAG_MC"."$tcgm"."$corrtagver
    echo $tagcorr
    # <== tag name
    for t in ${TYPES[@]}
    do
        [[ ${1:-0} -eq 1 ]] && {
            set -x
            ./reap_results $t $INPUT_MC $tagcorr 0 20 \
                           0 ${cgm:0:1} ${cgm:1:1} ${cgm:2:1} "null" \
                           2>&1 | tee logs/$tagcorr-$t.txt & # \
                # $multhandle $maxdr2 0 "null" \
                # $ctable "(1)" 
            set +x
        }
    done # for t in ${TYPES[@]}
    wait
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
        # tages=$nominal".m."$tagver".s."$cmin"."$cmax ;
        tages=$nominal".m."$corrtagver".s."$cmin"."$cmax ;
        [[ $TAG_MC == *${nominal}* || ${2:-0} -eq 2 ]] && {
            # ==> tag name
            tages=$TAG_MC"."$tcgm"."$tagver".s."$cmin"."$cmax
            # [[ $cmin -eq 0 && $cmax -eq 20 ]] && tages="incl."$TAG_MC"."$tcgm"."$tagver
            echo $tages
            # <== tag name
            [[ ${2:-0} -gt 0 ]] && {
                for t in ${TYPES[@]} ; do
                    set -x
                    ./reap_results $t $INPUT_MC $tages $cmin $cmax \
                                   0 ${cgm:0:1} ${cgm:1:1} ${cgm:2:1} "null" \
                                   $multhandle $maxdr2 0 "null" \
                                   $CENT_MC "(1)" \
                                   2>&1 | tee logs/$tages-$t.txt & # \
                        set +x
                done # for t in ${TYPES[@]}
                wait
            }

            trash figs/acceptance/accep-$tages-*.png
            [[ $cmin -eq 0 && $cmax -eq 20 ]] || {
                trash figs/corrections/alpha-$tages-*.png \
                      figs/corrections/sdfrac-$tages-*.png \
                      figs/corrections/trigger-$tages-*.png \
                      figspdf/fits/alphafit-$tages-*.pdf
            }
        }

        ##############################################
        #      apply correction, w. centrality       #
        ##############################################

        for dd in ${INPUTS_DATA[@]} ; do
            
            IFS=',' ; VINPUTS_DATA=($dd) ; unset IFS ;
            INPUT_DATA=${VINPUTS_DATA[0]}
            TAG_DATA=${VINPUTS_DATA[1]}
            CENT_DATA=${VINPUTS_DATA[2]}

            # echo " "$INPUT_DATA
            # echo " "$TAG_DATA
            
            tcgm=cgm # correction, geometric, acceptance map
            # [[ $TAG_DATA == *CLOSE ]] && { tcgm=cm ; tages="incl."${TAG_DATA%%CLOSE}".m."$tagver ; }
            [[ $TAG_DATA == *CLOSE ]] && { tcgm=cm ; tages=${TAG_DATA%%CLOSE}".m."$tagver".s."$cmin"."$cmax ; }
            [[ "12 13 14 23 24 34 56 57 67" =~ "${TYPES[0]}" ]] || tcgm=cm
            cgm=$(getcgm $tcgm)

            # ==> tag name
            # tagappl=$TAG_DATA"."$tcgm"."${tagcorr##incl.}".s."$cmin"."$cmax
            tagappl=$TAG_DATA"."$tcgm"."$TAG_MC".m."$tagver".s."$cmin"."$cmax
            echo $tagappl
            echo $tages
            # <== tag name
            [[ ${3:-0} -eq 1 ]] && {
                for t in ${TYPES[@]} ; do
                    set -x
                    ./reap_results $t $INPUT_DATA $tagappl $cmin $cmax \
                                   $tagcorr ${cgm:0:1} ${cgm:1:1} ${cgm:2:1} $tages \
                                   $multhandle $maxdr2 $tagdr "null" \
                                   $CENT_DATA "(1)" \
                                   2>&1 | tee logs/$tagappl-$t.txt & # \
                        set +x
                done # for t in ${TYPES[@]}
                wait
            }

            ##############################################
            #    average combinations, w. centrality     #
            ##############################################

            mergecomb=
            for t in ${TYPES[@]} ; do
                mergecomb=$mergecomb","$t
            done
            mergecomb=${mergecomb##,}
            [[ ${4:-0} -eq 1 ]] && {
                # truth="hijing.m.v0.s."$cmin"."$cmax"&"${taglabel[hijing]}"&2","amptnm.m.v0.s."$cmin"."$cmax"&"${taglabel[amptnm]}"&6","amptnm2.m.v0.s."$cmin"."$cmax"&"${taglabel[amptnm2]}"&4","hydjet.m.v0.s."$cmin"."$cmax"&"${taglabel[hydjet]}"&1","angantyr.m.v0.s."$cmin"."$cmax"&"${taglabel[angantyr]}"&8"
                truth="hijing.m.v0.s."$cmin"."$cmax"&"${taglabel[hijing]}"&2","amptnm.m.v0.s."$cmin"."$cmax"&"${taglabel[amptnm]}"&6","amptnm2.m.v0.s."$cmin"."$cmax"&"${taglabel[amptnm2]}"&4","hydjet.m.v0.s."$cmin"."$cmax"&"${taglabel[hydjet]}"&9"
                # [[ $TAG_DATA == *CLOSE* ]] && {
                #     truth=${TAG_DATA%%CLOSE}".m."$tagver".s."$cmin"."$cmax"&"${taglabel[${TAG_DATA%%CLOSE}]}"&2" ; 
                #     # [[ $cmin -eq 0 && $cmax -eq 20 ]] &&
                #         # { truth="incl."${TAG_DATA%%CLOSE}".m.v0&"${taglabel[${TAG_DATA%%CLOSE}]}"&1" ; }
                # }
                ./merge_monde $tagappl "${taglabel[${TAG_DATA%%CLOSE}]} corr. w. ${taglabel[$TAG_MC]}" $mergecomb "$truth"
            }
            
        done # for dd in ${INPUTS_DATA[@]}

        c=$((c+2))

    done # while [ $c -lt $((${#CENTS[@]}-1)) ]

done # for mm in ${INPUTS_MC[@]}
