#!/bin/bash

make merge_first || exit 1
# ./merge_first /afs/cern.ch/work/w/wangj/RECO2022/CMSSW_12_5_2/src/tracklet/analysis/output/correction-Hydjet-12.root output/correction-362294t
./merge_first output/correction-Hydjet_drlt0p5-12.root output/correction-362294t_drlt0p5
