#!/bin/bash

if [[ $# -ne 4 ]]; then
    echo "usage: ./tt-condor-checkfile.sh [input dir] [output dir] [max jobs] [log dir] [ishi] [evtfilt] [hltfilt] [mvafilt] [ptcut] [removeevent]"
    exit 1
fi

FILELIST=$1
DESTINATION=$2
MAXFILES=$3
LOGDIR=$4

PROXYFILE=$(ls /tmp/ -lt | grep $USER | grep -m 1 x509 | awk '{print $NF}')

tag="tracklet"

DEST_CONDOR=${DESTINATION}

if [ ! -d $DESTINATION ]
then
    if [[ $DESTINATION == /mnt/T2_US_MIT/* ]]
    then
        SRM_PREFIX="/mnt/T2_US_MIT/hadoop/" ; SRM_PATH=${DEST_CONDOR#${SRM_PREFIX}} ;
        # gfal-mkdir -p srm://se01.cmsaf.mit.edu:8443/srm/v2/server?SFN=$DESTINATION
        LD_LIBRARY_PATH='' PYTHONPATH='' gfal-mkdir -p gsiftp://se01.cmsaf.mit.edu:2811/${SRM_PATH} # default for T2 hadoop output
    else
        mkdir -p $DESTINATION
    fi
fi

mkdir -p $LOGDIR

counter=0
for i in `cat $FILELIST`
do
    if [ $counter -ge $MAXFILES ]
    then
        break
    fi
    inputname=${i}
    infn=${inputname##*/}
    infn=${infn%%.*} # no .root
    outputfile=${tag}_${infn}.root
    if [ ! -f ${DESTINATION}/${outputfile} ]
    then
        echo -e "\033[38;5;242mSubmitting a job for output\033[0m ${DESTINATION}/${outputfile}"
        
        cat > tt-${tag}.condor <<EOF

Universe     = vanilla
Initialdir   = $PWD/
Notification = Error
Executable   = $PWD/tt-${tag}-checkfile.sh
Arguments    = $inputname $DEST_CONDOR ${outputfile} $PROXYFILE 
GetEnv       = True
Output       = $LOGDIR/log-${infn}.out
Error        = $LOGDIR/log-${infn}.err
Log          = $LOGDIR/log-${infn}.log
Rank         = Mips
+AccountingGroup = "group_cmshi.$(whoami)"
Requirements = ( BOSCOCluster =!= "t3serv008.mit.edu" && BOSCOCluster =!= "ce03.cmsaf.mit.edu" && BOSCOCluster =!= "eofe8.mit.edu")
+DESIRED_Sites = "mit_tier2"
job_lease_duration = 240
should_transfer_files = YES
transfer_input_files = transmute_trees,/tmp/$PROXYFILE
Queue 
EOF

condor_submit tt-${tag}.condor -name submit04.mit.edu
mv tt-${tag}.condor $LOGDIR/log-${infn}.condor
counter=$(($counter+1))
    fi
done

echo -e "Submitted \033[1;36m$counter\033[0m jobs to Condor."
