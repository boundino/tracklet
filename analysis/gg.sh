
make gen_match || exit 1
make gun_study || exit 1


# ./gen_match /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/guns/tt_pixelsim_fullreco.root /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/guns/gmatch_pixelsim_fullreco.root
./gun_study /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/guns/gmatch_pixelsim_fullreco.root
# ./gun_study data/guns.root /eos/cms/store/group/cmst3/user/wangj/tracklet/tt_230724_pixel_230724_EposLHC_ReggeGribovParton_5360GeV_1255p1.root
