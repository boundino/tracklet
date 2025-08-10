
make merge || exit 1

CLUS=0 # 0: default; 1: clus; 3: randomvz
NFDIV=10 # only merge 1/NFDIV of full stats

LIST=(
    # after smiling cut
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250806_pixel_250806_IonPhysics0_394153_mask
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250806_randomvz_pixel_250806_IonPhysics0_394153_mask
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250806_weight_pixel_250806_Hijing_OO_5362GeV_pf_realistic_smile
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250806_randomvz_pixel_250806_Hijing_OO_5362GeV_pf_realistic_smile
    # after mask    
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250727_pixel_250727_IonPhysics0_394153_mask
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250727_randomvz_pixel_250727_IonPhysics0_394153_mask
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250727_weight_pixel_250727_Hijing_OO_5362GeV_pf_realistic_mask
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250727_randomvz_pixel_250727_Hijing_OO_5362GeV_pf_realistic_mask
    # before mask
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250718_pixel_250708_IonPhysics0_394153
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/tt_250709_randomvz_pixel_250708_IonPhysics0_394153
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250718_pixel_250715_Hijing_OO_5362GeV_pf_realistic
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250718_randomvz_pixel_250715_Hijing_OO_5362GeV_pf_realistic
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250718_weight_pixel_250715_Hijing_OO_5362GeV_pf_realistic
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250718_weight_pixel_250715_Pythia_Angantyr_OO_5362GeV_pf_realistic
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250718_weight_pixel_250715_AMPT_StringMelting_OO_5362GeV_pf_realistic
    
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_AMPT_NoStringMelting_OO_5360GeV_0527_v2
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_AMPT_StringMelting_OO_5360GeV_0527_v2
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_Hijing_OO_5360GeV_0527_v2
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_Hydjet_Drum5F_OO_5360GeV_0527_v2
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_AMPT_NoStringMeltingp0_OO_5360GeV_0527_v2
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_pixel_250528_EposLHC_ReggeGribovParton_OO_5360GeV_0527_v2
    
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_randomvz_pixel_250528_AMPT_NoStringMelting_OO_5360GeV_0527_v2
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_randomvz_pixel_250528_AMPT_StringMelting_OO_5360GeV_0527_v2
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_randomvz_pixel_250528_Hijing_OO_5360GeV_0527_v2
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250528_randomvz_pixel_250528_AMPT_NoStringMeltingp0_OO_5360GeV_0527_v2
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2025/private/tt_250615_pixel_250528_Hijing_OO_5360GeV_0527_v2 
)

# outputdir=/eos/cms/store/group/phys_heavyions/wangj/tracklet2025
mkdir -p filelists

for infile in ${LIST[@]} ; do
    l=${infile##*/}
    outputdir=${infile%%"$l"}
    ls --color=no -d ${infile}/* > filelists/${l}.txt
    outputname=${outputdir}/${l}
    [[ $CLUS -eq 1 ]] && {
        outputname=${outputname}_clus
    }
    [[ $NFDIV -gt 1 ]] && {
        outputname=${outputname}_p$NFDIV
    }
    outputname=${outputname}.root
    echo "./merge ${outputname} filelists/${l}.txt $CLUS $NFDIV"
    set -x
    ./merge ${outputname} filelists/${l}.txt $CLUS $NFDIV
    set +x
done
