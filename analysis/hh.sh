#!/bin/bash

make harvest_hists || exit 1
make evaluate_effs || exit 1

RUN_PIX_2D=1 ; TAG_PIX_COMPARE=362294a ;
RUN_TRK_2D=1
RUN_PIX_1D=1 ; TAG_PIX=362294a
RUN_TRK_1D=1 ; TAG_TRACKLET=362294a
RUN_VTX_EFF=1 ; TAG_VERTEX=harvest

. tool.shinc 

INPUTS_PIX=(
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/pixel_230126_HITestRaw0_HIRun2022A_MBPVfilTh4_362294.root,362294,362294a"
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/pixel_230129_EposLHC_ReggeGribovParton_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,${taglabel[epos]},epos"
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/pixel_230129_Hydjet_Drum5F_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,${taglabel[hydjet]},hydjet"
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/pixel_230129_AMPT_StringMelting_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,${taglabel[amptsm]},amptsm"
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/vpresplit/pixelpre_221229_AMPT_NoStringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,${taglabel[amptnm]},amptnm"
)

INPUTS_TRACKLET=(
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230322_pixel_230126_HITestRaw0-6_HIRun2022A_MBPVfilTh4_362294.root,362294,362294a"
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230322_pixel_230129_EposLHC_ReggeGribovParton_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,${taglabel[epos]},epos"
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230322_pixel_230129_Hydjet_Drum5F_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,${taglabel[hydjet]},hydjet"
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230322_pixel_230129_AMPT_StringMelting_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,${taglabel[amptsm]},amptsm"
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/vpresplit/tt_221229_vzshift_pixelpre_221229_AMPT_NoStringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,${taglabel[amptnm]},amptnm"
)

INPUTS_PIX_2D=()
for i in "${INPUTS_PIX[@]}"
do
    IFS="," ; inputs=($i) ; unset IFS ;
    ARG_PIX_INPUT=${ARG_PIX_INPUT}" "${inputs[0]}
    ARG_PIX_LEG=${ARG_PIX_LEG}"${inputs[1]},"

    [[ ${inputs[2]} == *hydjet* || ${inputs[2]} == *ampt* ]] && continue
    
    INPUTS_PIX_2D+=("$i")
    ARG_PIX_TAG_2D=${ARG_PIX_TAG_2D}" "${inputs[2]}
done
ARG_PIX_LEG=${ARG_PIX_LEG%%,}

for i in "${INPUTS_TRACKLET[@]}"
do
    IFS="," ; inputs=($i) ; unset IFS ;
    ARG_TRACKLET_INPUT=${ARG_TRACKLET_INPUT}" "${inputs[0]}
    ARG_TRACKLET_LEG=${ARG_TRACKLET_LEG}"${inputs[1]},"
done
ARG_TRACKLET_LEG=${ARG_TRACKLET_LEG%%,}

for i in "${INPUTS_TRACKLET[@]}"
do
    IFS="," ; inputs=($i) ; unset IFS ;
    [[ ${inputs[2]} == *a ]] && continue
    ARG_VERTEX_INPUT=${ARG_VERTEX_INPUT}" "${inputs[0]}
    ARG_VERTEX_LEG=${ARG_VERTEX_LEG}"${inputs[1]},"
done
ARG_VERTEX_LEG=${ARG_VERTEX_LEG%%,}

echo 'token
std::vector<std::string> files ='$ARG_PIX_INPUT'
token ,
std::vector<std::string> legends ='$ARG_PIX_LEG'
' > lists/pixel.list

echo 'token
std::vector<std::string> tags ='$ARG_PIX_TAG_2D'
' > lists/pixel-2d.list

echo 'token
std::vector<std::string> files ='$ARG_TRACKLET_INPUT'
token ,
std::vector<std::string> legends ='$ARG_TRACKLET_LEG'
' > lists/tracklet.list

echo 'token
std::vector<std::string> files ='$ARG_VERTEX_INPUT'
token ,
std::vector<std::string> legends ='$ARG_VERTEX_LEG'
' > lists/vertex.list

[[ $RUN_PIX_2D -eq 1 ]] && {
    ## Pixel 2D
    for i in "${INPUTS_PIX_2D[@]}"
    do
        IFS="," ; inputs=($i) ; unset IFS ;
        ./harvest_hists 2 ${inputs[0]} "${inputs[2]}"
    done

    ./harvest_hists 4 lists/pixel-2d.list $TAG_PIX_COMPARE
}

[[ $RUN_TRK_2D -eq 1 ]] && {
    ## Tracklet 2D
    for i in "${INPUTS_TRACKLET[@]}"
    do
        IFS="," ; inputs=($i) ; unset IFS ;
        ./harvest_hists 3 ${inputs[0]} "${inputs[2]}"
    done
}

## Pixel 1D
[[ $RUN_PIX_1D -eq 1 ]] && {
    ./harvest_hists 0 lists/pixel.list $TAG_PIX
}

## Tracklet 1D
[[ $RUN_TRK_1D -eq 1 ]] && {
    ./harvest_hists 1 lists/tracklet.list $TAG_TRACKLET
}

## Vertex eff
[[ $RUN_VTX_EFF -eq 1 ]] && {
    ./evaluate_effs lists/vertex.list $TAG_VERTEX
}

