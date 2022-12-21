#!/bin/bash

if [[ $# -ne 4 ]]; then
    echo "usage: ./skim-skimd-checkfile.sh [input file] [output dir] [output filename] [ishi] [evtfilt] [hltfilt] [mvafilt] [ptcut] [removeevent] [proxy]"
    exit 1
fi

INFILE=$1
DESTINATION=$2
OUTFILE=$3
export X509_USER_PROXY=${PWD}/${4}
echo $X509_USER_PROXY

set -x
./transmute_trees $INFILE $OUTFILE

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

rm $OUTFILE
