#!/bin/bash

if [[ $0 != *.sh ]]
then
    echo -e "\e[31;1merror:\e[0m use \e[32;1m./script.sh\e[0m instead of \e[32;1msource script.sh\e[0m"
    return 1
fi

#
MAXFILENO=5

#
movetosubmit=${1:-0}
runjobs=${2:-0}

PRIMARY="tt_221218_vzshift"
# PRIMARY="tt_221218_vzshift_reweight"

INPUTS=(
    "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/MinBias_Hydjet_Drum5F_PbPb_5360GeV/crab_pixelpre_221216_Hydjet_Drum5F_PbPb_5360GeV_1252p1_privateZ_Th4/221216_185332/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/MB_Hydjet_Run3_GENSIM/crab_pixelpre_221207_MB_Hydjet_Run3_subehera_Th4/221207_170609/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw0/crab_pixelpre_221201_HITestRaw0_HIRun2022A_MBPVfilTh4_362294/221201_192201/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw0/crab_pixelpre_221201_HITestRaw0_HIRun2022A_MBPVfilTh4_362318/221205_024532/000*/"
)

OUTPUTPRIDIR="/mnt/T2_US_MIT/hadoop/cms/store/user/jwang/tracklet/"
WORKDIR="/work/submit/$USER/tracklet/"

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
        ls --color=no $INPUTDIR/* -d | sed -e "s/\/mnt\/T2_US_MIT\/hadoop\/cms/root:\/\/cms-xrd-global.cern.ch\//g" > $INPUTFILELIST
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
        ./tt-condor-checkfile.sh "$INPUTFILELIST" $OUTPUTDIR $MAXFILENO $LOGDIR 
        set +x
    fi

done

if [ "$movetosubmit" -eq 1 ]
then
    cd ../
    make transmute_trees || exit 1
    cd condor

    mv -v ../transmute_trees $WORKDIR/
    cp -v $0 $WORKDIR/
    cp -v tt-tracklet-checkfile.sh $WORKDIR/
    cp -v tt-condor-checkfile.sh $WORKDIR/
fi

