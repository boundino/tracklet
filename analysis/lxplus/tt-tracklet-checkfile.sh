#!/bin/bash

if [[ $# -ne 7 ]]; then
    echo "usage: ./tt-tracklet-checkfile.sh [input file] [output dir] [output filename] [random] [split] [drop] [weight]"
    exit 1
fi

INFILE=$1
DESTINATION=$2
OUTFILE=$3
USERANDOM=$4
USESPLIT=$5
USEDROP=$6
USEWEIGHT=$7
# RELEASE=CMSSW_12_5_5_patch1

echo $SCRAM_ARCH

tar -xzvf tracklet.tar.gz
g++ transmute_trees.C $(root-config --libs --cflags) -g -o transmute_trees

# source /cvmfs/cms.cern.ch/cmsset_default.sh

# scram p CMSSW $RELEASE
# cd $RELEASE/src
# eval `scram runtime -sh`
# cd -

set -x

root --version

xrdcp $INFILE .

input_file=$INFILE
[[ -f $PWD/${INFILE##*/} ]] && input_file=$PWD/${INFILE##*/} || echo "xrdcp failed."
reweight=0
[[ $USEWEIGHT -gt -1 ]] && { reweight=1 ; }
sample=$USEWEIGHT
./transmute_trees $input_file $OUTFILE 0 1000000000 $sample $reweight 0 $USERANDOM $USESPLIT $USEDROP

if [[ $(wc -c $OUTFILE | awk '{print $1}') -gt 700 ]]; then
    # gfal-copy
    # SRM_PREFIX="/mnt/T2_US_MIT/hadoop/" ; SRM_PATH=${DESTINATION#${SRM_PREFIX}} ;
    # LD_LIBRARY_PATH='' PYTHONPATH='' gfal-copy file://$PWD/${OUTFILE} gsiftp://se01.cmsaf.mit.edu:2811/${SRM_PATH}/${OUTFILE}

    # xrdcp
    SRM_PREFIX="/eos/cms/" ; SRM_PATH=${DESTINATION#${SRM_PREFIX}} ;
    xrdcp ${OUTFILE} root://eoscms.cern.ch//${SRM_PATH}/$OUTFILE
fi
set +x

rm -r transmute_trees.C transmute_trees include tracklet.tar.gz
rm $PWD/${INFILE##*/}
rm $OUTFILE
