#!/bin/bash

if [[ $0 != *.sh ]]
then
    echo -e "\e[31;1merror:\e[0m use \e[32;1m./script.sh\e[0m instead of \e[32;1msource script.sh\e[0m"
    return 1
fi

#
MAXFILENO=2
USERANDOM=0
USESPLIT=0
USEDROP=0
#
movetosubmit=${1:-0}
runjobs=${2:-0}

PRIMARY="tt_230724"
# PRIMARY="tt_230724_randomvz"
# PRIMARY="tt_230724_split"
# PRIMARY="tt_230724_drop"

INPUTS=(
    "filelists/filelist_pixel_230724_HITestRaw0_HIRun2022A_MBPVfilTh4_362294.txt"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/MinBias_Drum5F_5p36TeV_hydjet/crab_pixel_230724_Hydjet_Drum5F_5360GeV_HINPbPbAutumn22DR/230724_163528/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw0/crab_pixel_230724_HITestRaw0_HIRun2022A_MBPVfilTh4_362294/230724_150607/000*/"

    # # pre-approval #
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/MinBias_EposLHC_ReggeGribovParton_PbPb_5360GeV/crab_pixel_230512_EposLHC_ReggeGribovParton_PbPb_5360GeV_230322_GTv9Th4/230515_111547/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/MinBias_Hydjet_Drum5F_PbPb_5360GeV/crab_pixel_230512_Hydjet_Drum5F_PbPb_5360GeV_230322_GTv9Th4/230525_160137/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/MinBias_AMPT_StringMelting_PbPb_5360GeV/crab_pixel_230512_AMPT_StringMelting_PbPb_5360GeV_230322_GTv9Th4/230526_165052/000*/"

    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw1/crab_pixel_230126_HITestRaw1_HIRun2022A_MBPVfilTh4_362318/230614_005326/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw2/crab_pixel_230126_HITestRaw2_HIRun2022A_MBPVfilTh4_362318/230614_010146/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw3/crab_pixel_230126_HITestRaw3_HIRun2022A_MBPVfilTh4_362318/230614_010318/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw4/crab_pixel_230126_HITestRaw4_HIRun2022A_MBPVfilTh4_362318/230614_010433/000*/"
    
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw0/crab_pixel_230126_HITestRaw0_HIRun2022A_MBPVfilTh4_362294/230126_184212/100*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw1/crab_pixel_230126_HITestRaw1_HIRun2022A_MBPVfilTh4_362294/230126_214813/100*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw2/crab_pixel_230126_HITestRaw2_HIRun2022A_MBPVfilTh4_362294/230126_214952/100*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw3/crab_pixel_230126_HITestRaw3_HIRun2022A_MBPVfilTh4_362294/230126_215203/100*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw4/crab_pixel_230126_HITestRaw4_HIRun2022A_MBPVfilTh4_362294/230126_215745/100*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw5/crab_pixel_230126_HITestRaw5_HIRun2022A_MBPVfilTh4_362294/230126_215914/100*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw6/crab_pixel_230126_HITestRaw6_HIRun2022A_MBPVfilTh4_362294/230126_220022/100*/"

    # # obsolete #
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/MinBias_EposLHC_ReggeGribovParton_PbPb_5360GeV/crab_pixel_230129_EposLHC_ReggeGribovParton_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4/230131_141449/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/MinBias_AMPT_StringMelting_PbPb_5360GeV/crab_pixel_230129_AMPT_StringMelting_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4/230131_142216/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/MinBias_Hydjet_Drum5F_PbPb_5360GeV/crab_pixel_230129_Hydjet_Drum5F_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4/230129_205028/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/MinBias_Hydjet_Drum5F_PbPb_5360GeV/crab_pixel_230126_Hydjet_Drum5F_PbPb_5360GeV_221224_GTv7priZ0_GTv8Th4/230126_191822/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/MinBias_Hydjet_Drum5F_PbPb_5360GeV/crab_pixelpre_221229_Hydjet_Drum5F_PbPb_5360GeV_221224_GTv7priZ0_Th4/230106_125359/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/MinBias_EposLHC_ReggeGribovParton_PbPb_5360GeV/crab_pixelpre_221229_EposLHC_ReggeGribovParton_PbPb_5360GeV_221224_GTv7priZ0_Th4/230106_140953/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/MinBias_AMPT_NoStringMelting_PbPb_5360GeV/crab_pixelpre_221229_AMPT_NoStringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4/230106_143142/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/MinBias_AMPT_StringMelting_PbPb_5360GeV/crab_pixelpre_221229_AMPT_StringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4/230106_142846/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw0/crab_pixelpre_221201_HITestRaw0_HIRun2022A_MBPVfilTh4_362294/221201_192201/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw1/crab_pixelpre_221201_HITestRaw1_HIRun2022A_MBPVfilTh4_362294/221205_222829/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw2/crab_pixelpre_221201_HITestRaw2_HIRun2022A_MBPVfilTh4_362294/221222_173004/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw3/crab_pixelpre_221201_HITestRaw3_HIRun2022A_MBPVfilTh4_362294/221222_193613/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw4/crab_pixelpre_221201_HITestRaw4_HIRun2022A_MBPVfilTh4_362294/221222_200711/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw5/crab_pixelpre_221201_HITestRaw5_HIRun2022A_MBPVfilTh4_362294/221222_201003/000*/"
    # "/mnt/T2_US_MIT/hadoop/cms/store/user/wangj/HITestRaw6/crab_pixelpre_221201_HITestRaw6_HIRun2022A_MBPVfilTh4_362294/221222_201203/000*/"
)

OUTPUTPRIDIR="/eos/cms/store/cmst3/user/wangj/tracklet/"
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
        ls --color=no $INPUTDIR/* -d | sed -e "s/\/mnt\/T2_US_MIT\/hadoop\/cms/root:\/\/xrootd.cmsaf.mit.edu\//g" > $INPUTFILELIST
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
        ./tt-condor-checkfile.sh "$INPUTFILELIST" $OUTPUTDIR $MAXFILENO $LOGDIR $USERANDOM $USESPLIT $USEDROP
        set +x
    fi

done

if [ "$movetosubmit" -eq 1 ]
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

