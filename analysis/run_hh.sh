#!/bin/bash

make harvest_hists || exit 1

RUN=362294

INPUT_PIX_DATA=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rRun2022/pixelpre_221201_HITestRaw0_HIRun2022A_MBPVfilTh4_${RUN}.root
INPUT_PIX_HYDJET=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rHydjet/pixelpre_20221201_MB_Hydjet_Run3_subehera_Th4.root
INPUT_TRACKLET_DATA=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rRun2022/tt_20221204_pixelpre_221201_HITestRaw0_HIRun2022A_MBPVfilTh4_${RUN}.root
INPUT_TRACKLET_HYDJET=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rHydjet/tt_20221204_pixelpre_20221201_MB_Hydjet_Run3_subehera_Th4_f40.root

echo 'token
std::vector<std::string> files = '$INPUT_PIX_DATA' '$INPUT_PIX_HYDJET'
token ,
std::vector<std::string> legends = Run '${RUN}',HYDJET
' > lists/pixel_data_${RUN}.list

echo 'token
std::vector<std::string> files = '$INPUT_TRACKLET_DATA' '$INPUT_TRACKLET_HYDJET'
token ,
std::vector<std::string> legends = Run '${RUN}',HYDJET
' > lists/tracklet_data_${RUN}.list

./harvest_hists 0 lists/pixel_data_${RUN}.list ${RUN}t 
./harvest_hists 1 lists/tracklet_data_${RUN}.list ${RUN}t
./harvest_hists 2 $INPUT_PIX_DATA ${RUN}t 
./harvest_hists 3 $INPUT_TRACKLET_DATA ${RUN}t 
