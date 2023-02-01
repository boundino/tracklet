#!/bin/bash

make harvest_hists || exit 1

RUN_PIX_2D=0
RUN_TRK_2D=0
RUN_PIX_1D=1 ; TAG_PIX=362294t
RUN_TRK_1D=1 ; TAG_TRACKLET=362294t

INPUTS_PIX=(
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/pixel_230126_HITestRaw0_HIRun2022A_MBPVfilTh4_362294.root,362294,362294t"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/vpresplit/pixelpre_221201_HITestRaw0_HIRun2022A_MBPVfilTh4_362294.root,362294 (Presplit),362294t-presplit"
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/pixel_230129_Hydjet_Drum5F_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,HYDJET,Hydjet"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/v7/pixel_230126_Hydjet_Drum5F_PbPb_5360GeV_221224_GTv7priZ0_GTv8Th4.root,HYDJET (Old bad),Hydjet-v7"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/vpresplit/pixelpre_221229_Hydjet_Drum5F_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,HYDJET (Presplit),Hydjet-presplit"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/vpresplit/pixelpre_221229_EposLHC_ReggeGribovParton_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,EPOS LHC,Epos"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/vpresplit/pixelpre_221229_AMPT_StringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,Hydjet,AMPT (w. String Melt),AmptWsm"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/vpresplit/pixelpre_221229_AMPT_NoStringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,AMPT (w/o String Melt),AmptNsm"
)

INPUTS_TRACKLET=(
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230127_pixel_230126_HITestRaw0-5_HIRun2022A_MBPVfilTh4_362294.root,362294,362294t"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/vpresplit/tt_221218_pixelpre_221201_HITestRaw0-6_HIRun2022A_MBPVfilTh4_362294.root,362294 (Presplit),362294t-presplit"
    "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230130_pixel_230129_Hydjet_Drum5F_PbPb_5360GeV_230129_GTv8priZ0_GTv8Th4.root,HYDJET,Hydjet"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/v7/tt_230127_pixel_230126_Hydjet_Drum5F_PbPb_5360GeV_221224_GTv7priZ0_GTv8Th4.root,HYDJET (Old bad),Hydjet-v7"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_230129_bkg5_pixel_230126_Hydjet_Drum5F_PbPb_5360GeV_221224_GTv7priZ0_GTv8Th4.root,HYDJET (5% Background),Hydjet-bkg5"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/vpresplit/tt_221229_vzshift_pixelpre_221229_Hydjet_Drum5F_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,HYDJET (Presplit),Hydjet-presplit"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/vpresplit/tt_221229_vzshift_pixelpre_221229_EposLHC_ReggeGribovParton_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,EPOS LHC,Epos"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/vpresplit/tt_221229_vzshift_pixelpre_221229_AMPT_StringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,AMPT (w. String Melt),AmptWsm"
    # "/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/vpresplit/tt_221229_vzshift_pixelpre_221229_AMPT_NoStringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root,AMPT (w/o String Melt),AmptNsm"
)

ARG_PIX_INPUT=
ARG_PIX_LEG=
for i in "${INPUTS_PIX[@]}"
do
    IFS="," ; inputs=($i) ; unset IFS ;
    ARG_PIX_INPUT=${ARG_PIX_INPUT}" "${inputs[0]}
    ARG_PIX_LEG=${ARG_PIX_LEG}"${inputs[1]},"
done
ARG_PIX_LEG=${ARG_PIX_LEG%%,}

ARG_TRACKLET_INPUT=
ARG_TRACKLET_LEG=
for i in "${INPUTS_TRACKLET[@]}"
do
    IFS="," ; inputs=($i) ; unset IFS ;
    ARG_TRACKLET_INPUT=${ARG_TRACKLET_INPUT}" "${inputs[0]}
    ARG_TRACKLET_LEG=${ARG_TRACKLET_LEG}"${inputs[1]},"
done
ARG_TRACKLET_LEG=${ARG_TRACKLET_LEG%%,}

echo 'token
std::vector<std::string> files = '$ARG_PIX_INPUT'
token ,
std::vector<std::string> legends = '$ARG_PIX_LEG'
' > lists/pixel.list

echo 'token
std::vector<std::string> files = '$ARG_TRACKLET_INPUT'
token ,
std::vector<std::string> legends = '$ARG_TRACKLET_LEG'
' > lists/tracklet.list

[[ $RUN_PIX_2D -eq 1 ]] && {
    ## Pixel 2D
    for i in "${INPUTS_PIX[@]}"
    do
        IFS="," ; inputs=($i) ; unset IFS ;
        ./harvest_hists 2 ${inputs[0]} "${inputs[2]}"
    done
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
