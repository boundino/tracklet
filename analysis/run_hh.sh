#!/bin/bash

make harvest_hists || exit 1

# ./harvest_hists 0 lists/pixel.list 304906a 
# ./harvest_hists 1 lists/tracklet.list 304906a 

# ./harvest_hists 0 lists/pixel.list Presplit 
# ./harvest_hists 1 lists/tracklet.list Presplit
# ./harvest_hists 2 pixelsim_presplitting.root Presplit 
# ./harvest_hists 3 out.root Presplit 

RUN=362294

INPUT_PIX_DATA=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r${RUN}/HITestRaw0.root
INPUT_PIX_HYDJET=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rHydjet/subehera220910.root
INPUT_TRACKLET_DATA=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r${RUN}/ttHITestRaw0.root
INPUT_TRACKLET_HYDJET=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rHydjet/tt_20221123_tracklet_20221122_MB_Hydjet_Run3_subehera.root

echo 'std::vector<std::string> files = '$INPUT_PIX_DATA' '$INPUT_PIX_HYDJET'
std::vector<std::string> legends = data'${RUN}' HYDJET
' > lists/pixel_data_${RUN}.list

echo 'std::vector<std::string> files = '$INPUT_TRACKLET_DATA' '$INPUT_TRACKLET_HYDJET'
std::vector<std::string> legends = data'${RUN}' HYDJET
' > lists/tracklet_data_${RUN}.list

./harvest_hists 0 lists/pixel_data_${RUN}.list ${RUN}t 
./harvest_hists 1 lists/tracklet_data_${RUN}.list ${RUN}t
./harvest_hists 2 $INPUT_PIX_DATA ${RUN}t 
./harvest_hists 3 $INPUT_TRACKLET_DATA ${RUN}t 
