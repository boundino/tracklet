#!/bin/bash

if [[ $# -ne 6 ]]; then
    echo "usage: ./tt-tracklet-checkfile.sh [input file] [output dir] [output filename] [random] [split]"
    exit 1
fi

INFILE=$1
DESTINATION=$2
OUTFILE=$3
USERANDOM=$4
USESPLIT=$5
USEDROP=$6
RELEASE=CMSSW_12_5_2_patch1

source /cvmfs/cms.cern.ch/cmsset_default.sh

scram p CMSSW $RELEASE
cd $RELEASE/src
eval `scram runtime -sh`
cd -

set -x
xrdcp $INFILE .

input_file=$INFILE
[[ -f $PWD/${INFILE##*/} ]] && input_file=$PWD/${INFILE##*/}
./transmute_trees $input_file $OUTFILE 0 1000000000 -1 0 0 $USERANDOM $USESPLIT $USEDROP

if [[ $(wc -c $OUTFILE | awk '{print $1}') -gt 700 ]]; then
    if [[ $DESTINATION == /mnt/T2_US_MIT/* ]]
    then
        # gfal-copy
        # SRM_PREFIX="/mnt/T2_US_MIT/hadoop/" ; SRM_PATH=${DESTINATION#${SRM_PREFIX}} ;
        # LD_LIBRARY_PATH='' PYTHONPATH='' gfal-copy file://$PWD/${OUTFILE} gsiftp://se01.cmsaf.mit.edu:2811/${SRM_PATH}/${OUTFILE}

        # xrdcp
        SRM_PREFIX="/mnt/T2_US_MIT/hadoop/cms/" ; SRM_PATH=${DESTINATION#${SRM_PREFIX}} ;
        xrdcp ${OUTFILE} root://xrootd.cmsaf.mit.edu//${SRM_PATH}/
    fi
fi
set +x

rm $PWD/${INFILE##*/}
rm $OUTFILE
