#!/bin/bash

make comp_avg || exit 1

source tool.shinc 

##
CONFIGS=(
    # "362294.cgm.epos.m.v1.s.4.20&#Deltar < 0.5,362294.cgm.epos.m.v1-drlt0p4.s.4.20&#Deltar < 0.4,362294.cgm.epos.m.v1-drlt0p6.s.4.20&#Deltar < 0.6[dr-362294.cgm.epos.m.v1.s.4.20[362294 corr. w. ${taglabel[epos]}"
    # "362294.cgm.epos.m.v1-vz0.s.4.20&-15 < v_{z} < -10,362294.cgm.epos.m.v1-vz1.s.4.20&-10 < v_{z} < -5,362294.cgm.epos.m.v1-vz2.s.4.20&-5 < v_{z} < 0,362294.cgm.epos.m.v1-vz3.s.4.20&0 < v_{z} < 5,362294.cgm.epos.m.v1-vz4.s.4.20&5 < v_{z} < 10,362294.cgm.epos.m.v1-vz5.s.4.20&10 < v_{z} < 15[vz-362294.cgm.epos.m.v1.s.4.20[362294 corr. w. ${taglabel[epos]}"
    # "362294.cgm.epos.m.v1.s.4.20&${taglabel[epos]},362294.cgm.hydjet.m.v1.s.4.20&${taglabel[hydjet]},362294.cgm.amptsm.m.v1.s.4.20&${taglabel[amptsm]}[mc-362294.cgm.m.v1.s.4.20[362294 corr. w. different MC"
    # "362294.cgm.epos.m.v1.s.4.20&tracklet count.,362294.cm.epos.m.v1.s.4.20&cluster count.[method-362294.cgm.epos.m.v1.s.4.20[362294 corr. w. ${taglabel[epos]}"
    "362294.cgm.epos.m.v1.s.4.20&362294,362318.cgm.epos.m.v1.s.4.20&362318[run-cgm.epos.m.v1.s.4.20[Different run corr. w. ${taglabel[epos]}"
)

#
for mm in "${CONFIGS[@]}"
do
    IFS='[' ; VCONFIG=($mm) ; unset IFS ;
    input=${VCONFIG[0]}
    tag=${VCONFIG[1]}
    text=${VCONFIG[2]}

    echo $input
    echo $tag
    echo $text
    
    ./comp_avg "$input" $tag "$text"

done # for mm in ${CONFIGS[@]}


