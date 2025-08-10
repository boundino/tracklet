#!/bin/bash

if [[ $0 != *.sh ]]
then
    echo -e "\e[31;1merror:\e[0m use \e[32;1m./script.sh\e[0m instead of \e[32;1msource script.sh\e[0m"
    return 1
fi

#
MAXFILENO=10000
USERANDOM=1
USESPLIT=0
USEDROP=0
USEWEIGHT=-1
#
movetosubmit=${1:-0}
runjobs=${2:-0}

PRIMARY="tt_250727"
[[ $USERANDOM -eq 1 ]] && PRIMARY=${PRIMARY}_randomvz
[[ $USESPLIT -gt 0 ]] && PRIMARY=${PRIMARY}_split
[[ $USEDROP -gt 0 ]] && PRIMARY=${PRIMARY}_drop
[[ $USEWEIGHT -gt -1 ]] && PRIMARY=${PRIMARY}_weight

INPUTS=(
    # after smiling cut
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/IonPhysics0/crab_pixel_250806_IonPhysics0_394153_mask/250807_145922/0000/"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/MinBias_Hijing_OO_5362GeV/crab_pixel_250806_Hijing_OO_5362GeV_pf_realistic_smile/250807_163954/0000/"
    # after mask
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/IonPhysics0/crab_pixel_250727_IonPhysics0_394153_mask/250728_202022/0000/"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/MinBias_Hijing_OO_5362GeV/crab_pixel_250727_Hijing_OO_5362GeV_pf_realistic_mask/250728_202541/0000/"

    # before mask
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/IonPhysics0/crab_pixel_250708_IonPhysics0_394153/250708_132402/0000/"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/MinBias_Hijing_OO_5362GeV/crab_pixel_250715_Hijing_OO_5362GeV_pf_realistic/250717_123454/0000/"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/MinBias_Pythia_Angantyr_OO_5362GeV/crab_pixel_250715_Pythia_Angantyr_OO_5362GeV_pf_realistic/250719_162204/0000/"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/MinBias_AMPT_StringMelting_OO_5362GeV/crab_pixel_250715_AMPT_StringMelting_OO_5362GeV_pf_realistic/250719_100834/0000/"

    # preapproval
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/MinBias_Hijing_b015_OO_5362GeV/crab_pixel_250528_Hijing_OO_5360GeV_0527_v2/250528_164044/0000/"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/MinBias_AMPT_StringMelting_b015_OO_5362GeV/crab_pixel_250528_AMPT_StringMelting_OO_5360GeV_0527_v2/250528_163726/0000/"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/MinBias_AMPT_NoStringMelting_b015_OO_5362GeV/crab_pixel_250528_AMPT_NoStringMelting_OO_5360GeV_0527_v2/250528_210847/0000/"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/MinBias_Hydjet_Drum5F_b015_OO_5362GeV/crab_pixel_250528_Hydjet_Drum5F_OO_5360GeV_0527_v2/250529_120226/0000/"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/MinBias_AMPT_NoStringMeltingp0_b015_OO_5362GeV/crab_pixel_250528_AMPT_NoStringMeltingp0_OO_5360GeV_0527_v2/250603_214246/0000/"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/MinBias_EposLHC_ReggeGribovParton_b015_OO_5362GeV/crab_pixel_250528_EposLHC_ReggeGribovParton_OO_5360GeV_0527_v2/250623_154918/0000/"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/MinBias_Pythia_Angantyr_OO_5362GeV/crab_pixel_250626_Pythia_Angantyr_OO_5362GeV_0626/250626_142559/0000/"
)

OUTPUTPRIDIR="/eos/cms/store/group/phys_heavyions/wangj/tracklet2025"
OUTPUTPRIDIR=$OUTPUTPRIDIR"/private"
WORKDIR="$PWD"

#
mkdir -p $WORKDIR

for INPUTDIR in "${INPUTS[@]}"
do
    echo -e "\e[32;1m$INPUTDIR\e[0m"
    
    if [[ $INPUTDIR == *.txt ]]
    then
        INPUTFILELIST=$INPUTDIR 
    else
        CRABNAME=${INPUTDIR##*crab_} ; CRABNAME=${CRABNAME%%/*} ;
        INPUTFILELIST="$WORKDIR/filelists/filelist_"$CRABNAME".txt"
        if [[ $INPUTDIR == /mnt/T2_US_MIT/* ]] ; then
            ls --color=no $INPUTDIR/* -d | sed -e "s/\/mnt\/T2_US_MIT\/hadoop\/cms/root:\/\/xrootd.cmsaf.mit.edu\//g" > $INPUTFILELIST
        fi
        if [[ $INPUTDIR == /eos* ]] ; then
            ls --color=no $INPUTDIR/* -d | sed -e "s/\/eos/root:\/\/eoscms.cern.ch\/\/eos/g" > $INPUTFILELIST
        fi
    fi
    echo "$INPUTFILELIST"
    REQUESTNAME=${INPUTFILELIST##*filelist_} ; REQUESTNAME=${REQUESTNAME%%.txt} ;
    OUTPUTSUBDIR="${PRIMARY}_${REQUESTNAME}"

    ##
    OUTPUTDIR="${OUTPUTPRIDIR}/${OUTPUTSUBDIR}"
    LOGDIR="logs/log_${OUTPUTSUBDIR}"

    echo "$OUTPUTDIR"
    ##

    if [ "$runjobs" -eq 1 ]
    then 
        set -x
        ./tt-condor-checkfile.sh "$INPUTFILELIST" $OUTPUTDIR $MAXFILENO $LOGDIR $USERANDOM $USESPLIT $USEDROP $USEWEIGHT 
        set +x
    fi

done

if [[ "$movetosubmit" -eq 1 ]]
then
    # cd ../
    # make transmute_trees || exit 1
    # cd lxplus    
    # mv -v ../transmute_trees $WORKDIR/

    cd ../
    tar -czvf tracklet.tar.gz transmute_trees.C include
    cd lxplus
    
    mv ../tracklet.tar.gz .
    
fi

