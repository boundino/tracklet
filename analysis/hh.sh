#!/bin/bash

make harvest_hists || exit 1

RUN=362294

INPUT_PIX_DATA=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/pixelpre_221201_HITestRaw0_HIRun2022A_MBPVfilTh4_${RUN}.root
INPUT_PIX_HYDJET=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/pixelpre_221229_Hydjet_Drum5F_PbPb_5360GeV_221224_GTv7priZ0_Th4.root
INPUT_PIX_EPOS=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/pixelpre_221229_EposLHC_ReggeGribovParton_PbPb_5360GeV_221224_GTv7priZ0_Th4.root
INPUT_PIX_AMPTST=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/pixelpre_221229_AMPT_StringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root
INPUT_PIX_AMPTNO=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/pixelpre_221229_AMPT_NoStringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root

INPUT_TRACKLET_DATA=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221218_pixelpre_221201_HITestRaw0-6_HIRun2022A_MBPVfilTh4_${RUN}.root
INPUT_TRACKLET_HYDJET=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_Hydjet_Drum5F_PbPb_5360GeV_221224_GTv7priZ0_Th4.root
INPUT_TRACKLET_EPOS=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_EposLHC_ReggeGribovParton_PbPb_5360GeV_221224_GTv7priZ0_Th4.root
INPUT_TRACKLET_AMPTST=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_AMPT_StringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root
INPUT_TRACKLET_AMPTNO=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/tt_221229_vzshift_pixelpre_221229_AMPT_NoStringMelting_PbPb_5360GeV_221224_GTv7priZ0_Th4.root

echo 'token
std::vector<std::string> files = '$INPUT_PIX_DATA' '$INPUT_PIX_HYDJET' '$INPUT_PIX_EPOS' '$INPUT_PIX_AMPTST' '$INPUT_PIX_AMPTNO'
token ,
std::vector<std::string> legends = Run '${RUN}',HYDJET,EPOS,AMPT (String Melt),AMPT (No String Melt)
' > lists/pixel_data_${RUN}.list

echo 'token
std::vector<std::string> files = '$INPUT_TRACKLET_DATA' '$INPUT_TRACKLET_HYDJET' '$INPUT_TRACKLET_EPOS' '$INPUT_TRACKLET_AMPTST' '$INPUT_TRACKLET_AMPTNO'
token ,
std::vector<std::string> legends = Run '${RUN}',HYDJET,EPOS,AMPT (String Melt),AMPT (No String Melt)
' > lists/tracklet_data_${RUN}.list

./harvest_hists 0 lists/pixel_data_${RUN}.list ${RUN}t 
./harvest_hists 1 lists/tracklet_data_${RUN}.list ${RUN}t 
./harvest_hists 2 $INPUT_PIX_DATA ${RUN}t 
./harvest_hists 2 $INPUT_PIX_HYDJET Hydjet 
./harvest_hists 3 $INPUT_TRACKLET_DATA ${RUN}t 

