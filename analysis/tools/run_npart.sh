#!/bin/bash

make npart || exit 1

# MC table
inputs=(
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/MinBias_EposLHC_ReggeGribovParton_OO_5362GeV/crab_pixel_250809_MINI_0731_EposLHC_ReggeGribovParton_OO_5362GeV_pf_realistic_maskf.root,3,OO_epos_pf
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/MinBias_AMPT_NoStringMelting_OO_5362GeV/crab_pixel_250809_MINI_0731_AMPT_NoStringMelting_OO_5362GeV_pf_realistic_maskf.root,5,OO_amptnm_pf
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/MinBias_AMPT_StringMelting_OO_5362GeV/crab_pixel_250809_MINI_0731_AMPT_StringMelting_OO_5362GeV_pf_realistic_maskf.root,6,OO_amptsm_pf
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/MinBias_Hijing_OO_5362GeV/crab_pixel_250810_MINI_0731_Hijing_OO_5362GeV_pf_realistic_maskf.root,7,OO_hijing_pf
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/MinBias_Hydjet_Cello_OO_5362GeV/crab_pixel_250809_MINI_0731_Hydjet_Cello_OO_5362GeV_pf_realistic_maskf.root,4,OO_hydjet_pf
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/MinBias_Pythia_Angantyr_OO_5362GeV/crab_pixel_250809_MINI_0715_Pythia_Angantyr_OO_5362GeV_pf_realistic_maskf.root,9,OO_angantyr_pf
)

for i in ${inputs[@]}; do
    IFS=',' ; params=($i) ; unset IFS ;
    input_file=${params[0]}
    ctable=${params[1]}
    tag=${params[2]}
    set -x
    ./npart $input_file $ctable $tag
    set +x
done
