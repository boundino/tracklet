#!/bin/bash

make harvest_hists || exit 1
make evaluate_effs || exit 1

RUN_PIXEL_2D=1 ; TAG_PIXEL_COMPARE=362294a
RUN_TRACKLET_2D=0 ; TAG_TRACKLET_COMPARE=362294a
RUN_PIXEL_1D=0 ; TAG_PIXEL=362294a
RUN_TRACKLET_1D=0 ; TAG_TRACKLET=362294a
RUN_VTX_EFF=0 ; TAG_VERTEX=harvest

. tool.shinc 

INPUTS_PIXEL=(
    "/eos/cms/store/cmst3/user/wangj/tracklet/pixel_230724_HITestRaw0_HIRun2022A_MBPVfilTh4_362294.root,362294,362294a"
    "/eos/cms/store/cmst3/user/wangj/tracklet/pixel_230724_EposLHC_ReggeGribovParton_5360GeV_1255p1.root,${taglabel[epos]},epos"
    "/eos/cms/store/cmst3/user/wangj/tracklet/pixel_230724_Hydjet_Drum5F_5360GeV_HINPbPbAutumn22DR.root,${taglabel[hydjet]},hydjet"
    "/eos/cms/store/cmst3/user/wangj/tracklet/pixel_230724_AMPT_StringMelting_5360GeV_HINPbPbAutumn22DR_v4.root,${taglabel[amptsm]},amptsm"
    "/eos/cms/store/cmst3/user/wangj/tracklet/pixel_230724_AMPT_NoStringMelting_5360GeV_HINPbPbAutumn22DR_v4.root,${taglabel[amptnm]},amptnm"

    # # pre-approval
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/pixel_230126_HITestRaw0_HIRun2022A_MBPVfilTh4_362294.root,362294,362294a"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/pixel_230129_EposLHC_ReggeGribovParton_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,${taglabel[epos]},epos"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/pixel_230129_Hydjet_Drum5F_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,${taglabel[hydjet]},hydjet"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/pixel_230129_AMPT_StringMelting_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,${taglabel[amptsm]},amptsm"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/vpresplit/pixelpre_221229_AMPT_NoStringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,${taglabel[amptnm]},amptnm"
)

INPUTS_TRACKLET=(
    "/eos/cms/store/cmst3/user/wangj/tracklet/tt_230724_pixel_230724_HITestRaw0-6_HIRun2022A_MBPVfilTh4_362294.root,362294,362294a"
    "/eos/cms/store/cmst3/user/wangj/tracklet/tt_230724_pixel_230724_EposLHC_ReggeGribovParton_5360GeV_1255p1.root,${taglabel[epos]},epos"
    "/eos/cms/store/cmst3/user/wangj/tracklet/tt_230724_pixel_230724_Hydjet_Drum5F_5360GeV_HINPbPbAutumn22DR_shuf.root,${taglabel[hydjet]},hydjet"
    "/eos/cms/store/cmst3/user/wangj/tracklet/tt_230724_pixel_230724_AMPT_StringMelting_5360GeV_HINPbPbAutumn22DR_v4_shuf.root,${taglabel[amptsm]},amptsm"
    "/eos/cms/store/cmst3/user/wangj/tracklet/tt_230724_pixel_230724_AMPT_NoStringMelting_5360GeV_HINPbPbAutumn22DR_v4_shuf.root,${taglabel[amptnm]},amptnm"
    # # pre-approval
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230322_pixel_230126_HITestRaw0-6_HIRun2022A_MBPVfilTh4_362294.root,362294,362294a"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230612_pixel_230512_EposLHC_ReggeGribovParton_PbPb_5360GeV_230322_GTv9Th4.root,${taglabel[epos]},epos"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230322_pixel_230129_Hydjet_Drum5F_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,${taglabel[hydjet]},hydjet"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230322_pixel_230129_AMPT_StringMelting_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,${taglabel[amptsm]},amptsm"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/vpresplit/tt_221229_vzshift_pixelpre_221229_AMPT_NoStringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,${taglabel[amptnm]},amptnm"
)

INPUTS_PIXEL_2D=()
for i in "${INPUTS_PIXEL[@]}"
do
    IFS="," ; inputs=($i) ; unset IFS ;
    ARG_PIXEL_INPUT=${ARG_PIXEL_INPUT}" "${inputs[0]}
    ARG_PIXEL_LEG=${ARG_PIXEL_LEG}"${inputs[1]},"

    [[ ${inputs[2]} == *hydjet* || ${inputs[2]} == *ampt* ]] && continue
    # [[ ${inputs[2]} != *epos* ]] && continue
    
    INPUTS_PIXEL_2D+=("$i")
    ARG_PIXEL_TAG_2D=${ARG_PIXEL_TAG_2D}" "${inputs[2]}
done
ARG_PIXEL_LEG=${ARG_PIXEL_LEG%%,}

INPUTS_TRACKLET_2D=()
for i in "${INPUTS_TRACKLET[@]}"
do
    IFS="," ; inputs=($i) ; unset IFS ;
    ARG_TRACKLET_INPUT=${ARG_TRACKLET_INPUT}" "${inputs[0]}
    ARG_TRACKLET_LEG=${ARG_TRACKLET_LEG}"${inputs[1]},"

    [[ ${inputs[2]} == *hydjet* || ${inputs[2]} == *ampt* ]] && continue
    
    INPUTS_TRACKLET_2D+=("$i")
    ARG_TRACKLET_TAG_2D=${ARG_TRACKLET_TAG_2D}" "${inputs[2]}
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
        ./harvest_hists 2 ${inputs[0]} "${inputs[2]}"
    done

    ./harvest_hists 4 lists/pixel-2d.list $TAG_PIXEL_COMPARE
}

[[ $RUN_TRACKLET_2D -eq 1 ]] && {
    ## Tracklet 2D
    for i in "${INPUTS_TRACKLET[@]}"
    do
        IFS="," ; inputs=($i) ; unset IFS ;
        ./harvest_hists 3 ${inputs[0]} "${inputs[2]}"
    done

    ./harvest_hists 5 lists/tracklet-2d.list $TAG_TRACKLET_COMPARE
}

## Pixel 1D
[[ $RUN_PIXEL_1D -eq 1 ]] && {
    ./harvest_hists 0 lists/pixel.list $TAG_PIXEL
}

## Tracklet 1D
[[ $RUN_TRACKLET_1D -eq 1 ]] && {
    ./harvest_hists 1 lists/tracklet.list $TAG_TRACKLET
}

## Vertex eff
[[ $RUN_VTX_EFF -eq 1 ]] && {
    ./evaluate_effs lists/vertex.list $TAG_VERTEX
}

