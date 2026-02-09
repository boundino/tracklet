make transmute_trees || exit 1

# ./transmute_trees $INPUT $OUTPUT [start:0] [end:-1] [reweight_sample:-1] [reweight:0] [pileup:0.] [random:0] [split:0.] [drop:0] [smear:0]

INPUT=/eos/cms/store/group/phys_heavyions/wangj/mc/pixel_sim_pf.root
# OUTPUTDIR=$PWD
OUTPUTDIR=
TAG=${INPUT##*/}
DIR=${INPUT/${TAG}/}
TAG=${TAG%%.*}
TAG=${TAG/pixel_/}

[[ x$OUTPUTDIR == x ]] && OUTPUTDIR=$DIR

echo "DIR: "$DIR
echo "TAG: "$TAG
echo "OUTPUTDIR: "$OUTPUTDIR

# Default -> [ add XX pair in defines.h ]
echo "--- Default"
OUTPUT=$OUTPUTDIR/tt_${TAG}.root
echo $OUTPUT
[[ ${1:-0} -eq 1 ]] && {
    ./transmute_trees $INPUT $OUTPUT ;
}

echo "--- Reweight"
OUTPUT=$OUTPUTDIR/tt_weight_${TAG}.root
echo $OUTPUT
[[ ${2:-0} -eq 1 ]] && {
 #./transmute_trees [input] [output] [start:0] [end:-1] [reweight_sample:-1] [reweight:0] [pileup:0.] [random:0] [split:0.] [drop:0] [smear:0]
 ./transmute_trees  $INPUT  $OUTPUT  0         -1       0                    1            ;
}

# Random
echo "--- Random"
OUTPUT=$OUTPUTDIR/tt_random_${TAG}.root
echo $OUTPUT
[[ ${3:-0} -eq 1 ]] && {
 #./transmute_trees [input] [output] [start:0] [end:-1] [reweight_sample:-1] [reweight:0] [pileup:0.] [random:0] [split:0.] [drop:0] [smear:0]
 ./transmute_trees  $INPUT  $OUTPUT  0         -1       -1                   0            0           1          ;
}

# Split -> [ MC only ]
echo "--- Split"
OUTPUT=$OUTPUTDIR/tt_split_${TAG}.root
echo $OUTPUT
[[ ${4:0} -eq 1 ]] && {
 #./transmute_trees [input] [output] [start:0] [end:-1] [reweight_sample:-1] [reweight:0] [pileup:0.] [random:0] [split:0.] [drop:0] [smear:0]
 ./transmute_trees  $INPUT  $OUTPUT  0         -1       -1                   0            0           0          0.02       ;
}
