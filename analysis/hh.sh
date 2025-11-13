#!/bin/bash

make harvest_hists || exit 1

RUN_PIXEL_2D=1 ;
RUN_PIXEL_2D_COMPARE=0 ; TAG_PIXEL_COMPARE=398864a
RUN_PIXEL_1D=0 ;

. tool.shinc 

mkdir -p output/hh lists

INPUTS_PIXEL=(
    ../../pixeldata_pre.root,398864a
)

INPUTS_PIXEL_2D=()
for i in "${INPUTS_PIXEL[@]}"
do
    IFS="," ; inputs=($i) ; unset IFS ;
    INPUT=${inputs[0]}
    TAG=${inputs[1]}
    TLABEL=`ftaglabel $TAG`
    ARG_PIXEL_INPUT+=" "$INPUT
    ARG_PIXEL_LEG+="${TLABEL},"

    INPUTS_PIXEL_2D+=("$i") # INPUTS for pixel 2D map
    ARG_PIXEL_TAG_2D+=" "$TAG # INPUTS for pixel 2D comparison ratio
done
ARG_PIXEL_LEG=${ARG_PIXEL_LEG%%,}

echo 'token
std::vector<std::string> files ='$ARG_PIXEL_INPUT'
token ,
std::vector<std::string> legends ='$ARG_PIXEL_LEG'
' > lists/pixel.list

echo 'token
std::vector<std::string> tags ='$ARG_PIXEL_TAG_2D'
' > lists/pixel-2d.list

[[ $RUN_PIXEL_2D -eq 1 ]] && {
    ## Pixel 2D
    for i in "${INPUTS_PIXEL_2D[@]}"
    do
        IFS="," ; inputs=($i) ; unset IFS ;
        INPUT=${inputs[0]}
        TAG=${inputs[1]}
        ./harvest_hists 2 $INPUT $TAG
    done
}

[[ $RUN_PIXEL_2D_COMPARE -eq 1 ]] && {
    ### compare Pixel 2D
    ./harvest_hists 4 lists/pixel-2d.list $TAG_PIXEL_COMPARE
}

## Pixel 1D
[[ $RUN_PIXEL_1D -eq 1 ]] && {
    ./harvest_hists 0 lists/pixel.list $TAG_PIXEL_COMPARE
}
