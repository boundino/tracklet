make get_barycentre || exit 1

. ../tool.shinc 

INPUT_DATA=/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/IonPhysics0/crab_pixel_250708_IonPhysics0_394153.root
TAG_DATA=394153
INPUT_MC=/eos/cms/store/group/phys_heavyions/wangj/tracklet2025/MinBias_Hijing_OO_5362GeV/crab_pixel_250809_MINI_0731_Hijing_OO_5362GeV_pf_realistic_maskf.root
TAG_MC=hijing

# ./get_barycentre "${INPUT_DATA}##`ftaglabel $TAG_DATA`","${INPUT_MC}##`ftaglabel $TAG_MC`" "394153-bsJul18"
./get_barycentre "${INPUT_DATA}##`ftaglabel $TAG_DATA`","${INPUT_MC}##`ftaglabel $TAG_MC`" "394153-bsJul31"


