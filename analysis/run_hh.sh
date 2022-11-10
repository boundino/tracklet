#!/bin/bash

make harvest_hists || exit 1

# ./harvest_hists 0 lists/pixel.list 304906a 
# ./harvest_hists 1 lists/tracklet.list 304906a 

./harvest_hists 0 lists/pixel.list Presplit 
./harvest_hists 1 lists/tracklet.list Presplit
./harvest_hists 2 pixelsim_presplitting.root Presplit 
./harvest_hists 3 out.root Presplit 
