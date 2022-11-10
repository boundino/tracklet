make transmute_trees || exit 1
./transmute_trees ../../pixelsim_presplitting.root out.root
# sem --wait --id sem-grp-tt-def
# ls out.root.* > notes/tt-merge-out-2022-11-03-23_22_52
# nice -n 10 sem -j-4 --id sem-grp-tt-def --linebuffer ./transmute_trees ../../pixelsim_presplitting.root out.root.0 0 1024 \ &> notes/log-tt-out-10-0.log
# ./keep/ladd notes/tt-merge-out-2022-11-03-23_22_52 out.root
