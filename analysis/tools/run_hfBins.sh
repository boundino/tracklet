#!/bin/bash

make hfBins || exit 1

# MC table
inputs=(
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/MinBias_EposLHC_ReggeGribovParton_OO_5362GeV/crab_pixel_250809_MINI_0731_EposLHC_ReggeGribovParton_OO_5362GeV_pf_realistic_maskf.root,OO_epos_pf
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/MinBias_AMPT_NoStringMelting_OO_5362GeV/crab_pixel_250809_MINI_0731_AMPT_NoStringMelting_OO_5362GeV_pf_realistic_maskf.root,OO_amptnm_pf
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/MinBias_AMPT_StringMelting_OO_5362GeV/crab_pixel_250809_MINI_0731_AMPT_StringMelting_OO_5362GeV_pf_realistic_maskf.root,OO_amptsm_pf
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/MinBias_Hijing_OO_5362GeV/crab_pixel_250810_MINI_0731_Hijing_OO_5362GeV_pf_realistic_maskf.root,OO_hijing_pf
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/MinBias_Hydjet_Cello_OO_5362GeV/crab_pixel_250809_MINI_0731_Hydjet_Cello_OO_5362GeV_pf_realistic_maskf.root,OO_hydjet_pf
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/MinBias_Pythia_Angantyr_OO_5362GeV/crab_pixel_250809_MINI_0715_Pythia_Angantyr_OO_5362GeV_pf_realistic_maskf.root,OO_angantyr_pf
)

for i in ${inputs[@]}; do
    IFS=',' ; params=($i) ; unset IFS ;
    input_file=${params[0]}
    tag=${params[1]}
    set -x
    ./hfBins $input_file $tag
    set +x
done

# data table
./hfBins
