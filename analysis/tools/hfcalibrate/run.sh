#!/bin/bash

make hfcomb || exit 1
make hffill || exit 1



# INPUT_DATA=/eos/cms/store/cmst3/user/wangj/tracklet/tt_230724_pixel_230724_HITestRaw0-6_HIRun2022A_MBPVfilTh4_362294.root#TrackletTree12,Data
INPUT_DATA=/eos/cms/store/cmst3/user/wangj/tracklet/hf_230724_HITestRaw10-15_HIRun2022A_MBPVfilTh4_362294_362320.root#pixel/PixelTree,Data
INPUT_MC=/eos/cms/store/cmst3/user/wangj/tracklet/hf_230724_hydjet_Drum5F_5360GeV_HINPbPbAutumn22DR.root#pixel/PixelTree,HYDJET

for thresh in 100 50 300
do

    OUTPUT_COMB=output/hf_230724_HITestRaw10-15_HIRun2022A_MBPVfilTh4_362294_362320_HydjetTh${thresh}.root#PixelTree,Data+HYDJET
    output=hf2Th4_362294_HYDJET_thre${thresh}

    [[ ${1:-0} -eq 1 ]] && ./hfcomb $INPUT_DATA $INPUT_MC $OUTPUT_COMB $thresh
    [[ ${2:-0} -eq 1 ]] && ./hffill $INPUT_DATA $INPUT_MC $OUTPUT_COMB $output "hf2Th4Threshold$thresh"

done

[[ ${3:-0} -eq 1 ]] && ./hffill 
