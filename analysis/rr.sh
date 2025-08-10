 #!/bin/bash

make reap_results || exit 1
make merge_monde || exit 1

maxdr2=0.25 ; tagdr="drlt0p5" 
tagver="v1" ; nominal="hijing" ; corrtagver="v1"

TYPES=(12 13 14 23 24 34 56 57 67)
# TYPES=(11 22 33 44 55 66 77) ; tagver=$tagver"-clus" ; corrtagver=$corrtagver"-clus" ;
CENTS=(0 20) # (4 20)
for i in {20..4} ; do CENTS+=($((i-1)) $i) ; done ; 

##
INPUTS_MC=(
    # after smiling cut
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250806_weight_pixel_250806_Hijing_OO_5362GeV_pf_realistic_smile.root,hijing,7
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250806_weight_pixel_250806_Hijing_OO_5362GeV_pf_realistic_smile_clus.root,hijing,7
    # after mask
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250727_weight_pixel_250727_Hijing_OO_5362GeV_pf_realistic_mask.root,hijing,7
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250727_weight_pixel_250727_Hijing_OO_5362GeV_pf_realistic_mask_clus.root,hijing,7

    # before mask
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250715_pixel_250715_Hijing_OO_5362GeV_pf_realistic.root,hijing,7
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250718_weight_pixel_250715_Hijing_OO_5362GeV_pf_realistic.root,hijing,7
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250718_weight_pixel_250715_AMPT_StringMelting_OO_5362GeV_pf_realistic.root,amptsm,6
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250718_weight_pixel_250715_Pythia_Angantyr_OO_5362GeV_pf_realistic.root,angantyr,9
    # clus
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250718_pixel_250715_Hijing_OO_5362GeV_pf_realistic_clus.root,hijing,7

    # old BS
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_Hijing_OO_5360GeV_0527_v2.root,hijing,7
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_AMPT_NoStringMelting_OO_5360GeV_0527_v2.root,amptnm,5
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_AMPT_NoStringMeltingp0_OO_5360GeV_0527_v2.root,amptnm2,8 # !
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_Hydjet_Drum5F_OO_5360GeV_0527_v2.root,hydjet,4
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_AMPT_StringMelting_OO_5360GeV_0527_v2.root,amptsm,6
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_EposLHC_ReggeGribovParton_OO_5360GeV_0527_v2.root,epos,3
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250626_Pythia_Angantyr_OO_5362GeV_0626.root,angantyr,9
)

INPUTS_DATA=(
    # after smiling cut
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250806_pixel_250806_IonPhysics0_394153_mask_p10.root,394153,0
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250806_pixel_250806_IonPhysics0_394153_mask_clus_p10.root,394153,0
    # after mask
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250727_pixel_250727_IonPhysics0_394153_mask_p10.root,394153,0
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250727_pixel_250727_IonPhysics0_394153_mask_clus_p10.root,394153,0

    # before mask
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250709_pixel_250708_IonPhysics0_394153_p10.root,394153,0
    # clus
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250718_pixel_250708_IonPhysics0_394153_clus_p10.root,394153,0
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
        tages=$TAG_MC"."$tcgm"."$tagver".s."$cmin"."$cmax
        # tages=$nominal".m."$corrtagver".s."$cmin"."$cmax ;
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
                truth="hijing.m.v1.s."$cmin"."$cmax"&"${taglabel[hijing]}"&2","amptnm.m.v0.s."$cmin"."$cmax"&"${taglabel[amptnm]}"&6","amptsm.m.v0.s."$cmin"."$cmax"&"${taglabel[amptsm]}"&4","hydjet.m.v0.s."$cmin"."$cmax"&"${taglabel[hydjet]}"&9","angantyr.m.v0.s."$cmin"."$cmax"&"${taglabel[angantyr]}"&8","epos.m.v0.s."$cmin"."$cmax"&"${taglabel[epos]}"&10"
                # [[ $TAG_DATA == *CLOSE* ]] && {
                #     truth=${TAG_DATA%%CLOSE}".m."$tagver".s."$cmin"."$cmax"&"${taglabel[${TAG_DATA%%CLOSE}]}"&2" ; 
                #     # [[ $cmin -eq 0 && $cmax -eq 20 ]] &&
                #         # { truth="incl."${TAG_DATA%%CLOSE}".m.v0&"${taglabel[${TAG_DATA%%CLOSE}]}"&1" ; }
                # }
                ./merge_monde $tagappl "$(ftaglabel ${TAG_DATA%%CLOSE}) corr. w. ${taglabel[$TAG_MC]}" $mergecomb "$truth"
            }
            
        done # for dd in ${INPUTS_DATA[@]}

        c=$((c+2))

    done # while [ $c -lt $((${#CENTS[@]}-1)) ]

done # for mm in ${INPUTS_MC[@]}
