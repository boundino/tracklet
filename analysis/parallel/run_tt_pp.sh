#!/bin/bash

filelist=(
    /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r362294/HITestRaw0/pixeldata_prespl_03ff59ee-4b5f-4726-8f12-b1bacec1667d.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r362294/HITestRaw0/pixeldata_prespl_0ea79907-85a4-415b-915a-58ab2771a224.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r362294/HITestRaw0/pixeldata_prespl_31fee8ef-acfd-438d-a40b-9e99ef1ff906.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r362294/HITestRaw0/pixeldata_prespl_41a0deed-b66e-482b-84e2-0b8443b31140.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r362294/HITestRaw0/pixeldata_prespl_76b61dd4-f4be-4bc6-afbb-956bb31b0f72.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r362294/HITestRaw0/pixeldata_prespl_8012f4ef-02a6-44fa-9264-72039a97bc5c.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r362294/HITestRaw0/pixeldata_prespl_91d3a6e0-703b-408b-81b1-6792d727e16e.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r362294/HITestRaw0/pixeldata_prespl_98377dfe-3064-46cd-9152-20d1966d4010.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r362294/HITestRaw0/pixeldata_prespl_a2bcc43f-f722-440a-bf82-804afbbc31f1.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r362294/HITestRaw0/pixeldata_prespl_aef86815-4230-497c-bdc3-3f5313e61248.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r362294/HITestRaw0/pixeldata_prespl_bfe72ab5-2eb8-496f-b2ca-f89ec8d1153c.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r362294/HITestRaw0/pixeldata_prespl_e3c86ea3-61f8-4c41-9f58-13315a12efa1.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r362294/HITestRaw0/pixeldata_prespl_e6d89c69-5016-4e0c-93f7-dcf7f30ea912.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r362294/HITestRaw0/pixeldata_prespl_f9096b63-d8cf-42d6-b047-716090d3ec3f.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r362294/HITestRaw0/pixeldata_prespl_fbaef8bc-f55d-4445-a0f4-f6e66ae8323e.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r362294/HITestRaw0/pixeldata_prespl_fc283f2b-100e-45e3-a853-761711004eed.root
    # /eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r362294/HITestRaw0/pixeldata_prespl_fc2f7f9c-d67a-4fd8-aba3-49e47cffa191.root
)

make transmute_trees || exit 1

for ff in ${filelist[@]}
do
    INPUT=$ff
    OUTPUT=/eos/cms/store/group/phys_heavyions/wangj/tracklet2022/r362294/ttHITestRaw0/tt_${ff##*/}
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
