#!/bin/bash

make harvest_hists || exit 1

# ./harvest_hists 0 lists/pixel.list 304906a 
# ./harvest_hists 1 lists/tracklet.list 304906a 

# ./harvest_hists 0 lists/pixel.list Presplit 
# ./harvest_hists 1 lists/tracklet.list Presplit
# ./harvest_hists 2 pixelsim_presplitting.root Presplit 
# ./harvest_hists 3 out.root Presplit 

RUN=362294

echo 'std::vector<std::string> files = pixeldata_presplitting.root
std::vector<std::string> legends = Run'${RUN}'
' > lists/pixel_data_${RUN}.list

echo 'std::vector<std::string> files = outdata.root
std::vector<std::string> legends = Run'${RUN}'
' > lists/tracklet_data_${RUN}.list

./harvest_hists 0 lists/pixel_data_${RUN}.list ${RUN}t 
./harvest_hists 1 lists/tracklet_data_${RUN}.list ${RUN}t
./harvest_hists 2 pixeldata_presplitting.root ${RUN}t 
./harvest_hists 3 outdata.root ${RUN}t 
