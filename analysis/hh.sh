#!/bin/bash

make harvest_hists || exit 1
make evaluate_effs || exit 1

RUN_PIXEL_2D=0 ;
RUN_PIXEL_2D_COMPARE=1 ; TAG_PIXEL_COMPARE=394153a
RUN_TRACKLET_2D=0 ;
RUN_TRACKLET_2D_COMPARE=0 ; TAG_TRACKLET_COMPARE=394153a
RUN_PIXEL_1D=0 ;
RUN_TRACKLET_1D=0 ;
RUN_VTX_EFF=0 ;

. tool.shinc 

INPUTS_PIXEL=(
    # smiling cut
    # after mask
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/IonPhysics0/crab_pixel_250727_IonPhysics0_394153_mask.root,394153a"
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/MinBias_Hijing_OO_5362GeV/crab_pixel_250727_Hijing_OO_5362GeV_pf_realistic_mask.root,hijing"
    # before mask
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/IonPhysics0/crab_pixel_250708_IonPhysics0_394153.root,394153t"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/MinBias_Hijing_OO_5362GeV/crab_pixel_250715_Hijing_OO_5362GeV_pf_realistic.root,hijingt"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/MinBias_Pythia_Angantyr_OO_5362GeV/crab_pixel_250715_Pythia_Angantyr_OO_5362GeV_pf_realistic.root,angantyrt"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/MinBias_AMPT_StringMelting_OO_5362GeV/crab_pixel_250715_AMPT_StringMelting_OO_5362GeV_pf_realistic.root,amptsmt"
)

INPUTS_TRACKLET=(
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250709_pixel_250708_IonPhysics0_394153_p10.root,394153t"
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250718_weight_pixel_250715_Hijing_OO_5362GeV_pf_realistic.root,hijingt"
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250718_weight_pixel_250715_Pythia_Angantyr_OO_5362GeV_pf_realistic.root,angantyrt"
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250718_weight_pixel_250715_AMPT_StringMelting_OO_5362GeV_pf_realistic.root,amptsmt"
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

    [[ $TAG == 39* || $TAG == *hijing* ]] || continue
    
    INPUTS_PIXEL_2D+=("$i") # INPUTS for pixel 2D map
    ARG_PIXEL_TAG_2D+=" "$TAG # INPUTS for pixel 2D comparison ratio
done
ARG_PIXEL_LEG=${ARG_PIXEL_LEG%%,}

INPUTS_TRACKLET_2D=()
for i in "${INPUTS_TRACKLET[@]}"
do
    IFS="," ; inputs=($i) ; unset IFS ;
    INPUT=${inputs[0]}
    TAG=${inputs[1]}
    TLABEL=`ftaglabel $TAG`
    ARG_TRACKLET_INPUT+=" "$INPUT
    ARG_TRACKLET_LEG+="${TLABEL},"

    [[ $TAG == 39* || $TAG == *hijing* ]] || continue
    
    INPUTS_TRACKLET_2D+=("$i") # INPUTS for tracklet 2D map
    ARG_TRACKLET_TAG_2D+=" "$TAG # INPUTS for tracklet 2D comparison ratio
done
ARG_TRACKLET_LEG=${ARG_TRACKLET_LEG%%,}

for i in "${INPUTS_TRACKLET[@]}"
do
    IFS="," ; inputs=($i) ; unset IFS ;
    INPUT=${inputs[0]}
    TAG=${inputs[1]}
    TLABEL=`ftaglabel $TAG`

    # [[ $TAG == *a ]] && continue

    ARG_VERTEX_INPUT+=" "$INPUT
    ARG_VERTEX_LEG+="${TLABEL},"
done
ARG_VERTEX_LEG=${ARG_VERTEX_LEG%%,}

echo 'token
std::vector<std::string> files ='$ARG_PIXEL_INPUT'
token ,
std::vector<std::string> legends ='$ARG_PIXEL_LEG'
' > lists/pixel.list

echo 'token
std::vector<std::string> tags ='$ARG_PIXEL_TAG_2D'
' > lists/pixel-2d.list

echo 'token
std::vector<std::string> files ='$ARG_TRACKLET_INPUT'
token ,
std::vector<std::string> legends ='$ARG_TRACKLET_LEG'
' > lists/tracklet.list

echo 'token
std::vector<std::string> tags ='$ARG_TRACKLET_TAG_2D'
' > lists/tracklet-2d.list

echo 'token
std::vector<std::string> files ='$ARG_VERTEX_INPUT'
token ,
std::vector<std::string> legends ='$ARG_VERTEX_LEG'
' > lists/vertex.list

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

[[ $RUN_TRACKLET_2D -eq 1 ]] && {
    ## Tracklet 2D
    for i in "${INPUTS_TRACKLET_2D[@]}"
    do
        IFS="," ; inputs=($i) ; unset IFS ;
        INPUT=${inputs[0]}
        TAG=${inputs[1]}
        ./harvest_hists 3 $INPUT $TAG
    done
}
[[ $RUN_TRACKLET_2D_COMPARE -eq 1 ]] && {
    ### compare Tracklet 2D
    ./harvest_hists 5 lists/tracklet-2d.list $TAG_TRACKLET_COMPARE
}

## Pixel 1D
[[ $RUN_PIXEL_1D -eq 1 ]] && {
    ./harvest_hists 0 lists/pixel.list $TAG_PIXEL_COMPARE
}

## Tracklet 1D
[[ $RUN_TRACKLET_1D -eq 1 ]] && {
    ./harvest_hists 1 lists/tracklet.list $TAG_TRACKLET_COMPARE
}

## Vertex eff
[[ $RUN_VTX_EFF -eq 1 ]] && {
    ./evaluate_effs lists/vertex.list $TAG_TRACKLETE_COMPARE
}

