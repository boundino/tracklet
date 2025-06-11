make transmute_trees || exit 1

# ./transmute_trees $INPUT $OUTPUT [start:0] [end:-1] [reweight_sample:-1] [reweight:0] [pileup:0.] [random:0] [split:0.] [drop:0] [smear:0]

INPUT=/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/AMPT_NoPU_100kEvents_OO_5360GeV_GenSim_032525.root

TAG=${INPUT##*/}
DIR=${INPUT/${TAG}/}
TAG=${TAG%%.*}
TAG=${TAG/pixel_/}

echo "DIR: "$DIR
echo "TAG: "$TAG

# Default -> [ add XX pair in defines.h ]
echo "--- Default"
OUTPUT=$DIR/tt_${TAG}.root
echo $OUTPUT
[[ ${1:-0} -eq 1 ]] && {
    ./transmute_trees $INPUT $OUTPUT ;
}

# Random
echo "--- Random"
OUTPUT=$DIR/tt_${TAG}_randomz.root
echo $OUTPUT
[[ ${2:-0} -eq 1 ]] && {
    ./transmute_trees $INPUT $OUTPUT 0 -1 -1 0 0 1 ;
}

# Split -> [ MC only ]
echo "--- Split"
OUTPUT=$DIR/tt_${TAG}_split.root
echo $OUTPUT
[[ ${3:0} -eq 1 ]] && {
    ./transmute_trees $INPUT $OUTPUT 0 -1 -1 0 0 0 0.02 ;
}


# ./transmute_trees /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/AMPT_NoPU_100kEvents_OO_5360GeV_GenSim_032525.root /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_AMPT_NoPU_100kEvents_OO_5360GeV_GenSim_032525.root 
# ./transmute_trees /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/Hijing_NoPU_100kEvents_OO_5360GeV_GenSim_030825.root /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_Hijing_NoPU_100kEvents_OO_5360GeV_GenSim_030825_randomz.root 0 -1 -1 0 0 1
