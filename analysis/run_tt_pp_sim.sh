#!/bin/bash

filelist=(
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rHydjet/subehera220910/pixelsim_prespl_step3_apprximateCluster_100.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rHydjet/subehera220910/pixelsim_prespl_step3_apprximateCluster_101.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rHydjet/subehera220910/pixelsim_prespl_step3_apprximateCluster_102.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rHydjet/subehera220910/pixelsim_prespl_step3_apprximateCluster_103.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rHydjet/subehera220910/pixelsim_prespl_step3_apprximateCluster_104.root
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rHydjet/subehera220910/pixelsim_prespl_step3_apprximateCluster_105.root
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rHydjet/subehera220910/pixelsim_prespl_step3_apprximateCluster_106.root
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rHydjet/subehera220910/pixelsim_prespl_step3_apprximateCluster_107.root
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rHydjet/subehera220910/pixelsim_prespl_step3_apprximateCluster_108.root
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rHydjet/subehera220910/pixelsim_prespl_step3_apprximateCluster_109.root
)

make transmute_trees || exit 1

for ff in ${filelist[@]}
do
    INPUT=$ff
    OUTPUT=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/rHydjet/ttsubehera220910/tt_${ff##*/}
    echo $INPUT
    echo $OUTPUT

    ./transmute_trees $INPUT $OUTPUT &
done

wait

# ./transmute_trees ../../pixeldata_presplitting.root outdata.root
# sem --wait --id sem-grp-tt-def
# ls out.root.* > notes/tt-merge-out-2022-11-03-23_22_52
# nice -n 10 sem -j-4 --id sem-grp-tt-def --linebuffer ./transmute_trees ../../pixelsim_presplitting.root out.root.0 0 1024 \ &> notes/log-tt-out-10-0.log
# ./keep/ladd notes/tt-merge-out-2022-11-03-23_22_52 out.root
