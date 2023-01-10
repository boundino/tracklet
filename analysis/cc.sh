#!/bin/bash

make compile_cl || exit 1

tag="drlt0p5"
DIV="&"

inputs_mc=(
    "Hydjet,HYDJET,1"
    "Epos,EPOS,2"
    "AMPTstring,AMPT (String Melt),4"
    "AMPTnostring,AMPT (No String Melt),8"
)

inputs_data=(
    362294
)

input_corr=
input_truth=
for ii in "${inputs_mc[@]}"
do
    IFS="," ; iis=(${ii}) ; unset IFS ;
    name=${iis[0]}
    leg=${iis[1]}
    lstyle=${iis[2]}
    echo $name" ==> "$leg

    # Closure
    [[ ${1:-0} -eq 1 ]] && ./compile_cl "${name}Close-Hydjet_${tag}"$DIV"" "12,13,14,23,24,34,56,57,67" "${name}_${tag}"$DIV"Truth ${leg}"$DIV"1" 602,860,858,419,414,434,635,629,626 "${leg} corr w/ HYDJET" ${name}Close-Hydjet_${tag} $DIV
    input_corr=${input_corr}",362294t-${name}_${tag}"$DIV"corr w/ ${leg}" 
    input_truth=${input_truth}",${name}_${tag}"$DIV"${leg}"$DIV"${lstyle}" 

done

input_corr=${input_corr##,}
input_truth=${input_truth##,}

echo $input_corr
echo $input_truth

for ii in "${inputs_data[@]}"
do
    # Combination
    for jj in "${inputs_mc[@]}"
    do
        IFS="," ; jjs=(${jj}) ; unset IFS ;
        name=${jjs[0]}
        leg=${jjs[1]}
        [[ ${2:-0} -eq 1 ]] && ./compile_cl "${ii}t-${name}_${tag}"$DIV"" "12,13,14,23,24,34,56,57,67" "$input_truth" 602,860,858,419,414,434,635,629,626 "Run ${ii} corr w/ ${leg}" ${ii}t-${name}_${tag} $DIV
    done

    # Compare MC corr
    for tt in 12 13 14 23 24 34 56 57 67
    do
        echo $tt
        [[ ${3:-0} -eq 1 ]] && ./compile_cl "$input_corr" $tt "$input_truth" 602,860,858,419 "Run ${ii}" "${ii}t-MC_${tag}-${tt}" $DIV
    done
done



# ./compile_cl "362294t-Hydjet_drlt0p5#HYDJET corr,362294t-Epos_drlt0p5#EPOS corr" "12" "AMPTnostring_drlt0p5#AMPT (No String)#8,AMPTstring_drlt0p5#AMPT (String Melt)#5,Hydjet_drlt0p5#HYDJET#1,Epos_drlt0p5#EPOS#2" 2,3,4,5 compiletest 
# ./compile_cl "EposClose-Hydjet_drlt0p5#" "12,13,14,23,24,34,56,57,67" "Epos_drlt0p5#Truth EPOS#1" 602,860,858,419,414,434,635,629,626 "EPOS corr w/ HYDJET" EposClose-Hydjet_drlt0p5
# ./compile_cl "AMPTstringClose-Hydjet_drlt0p5#" "12,13,14,23,24,34,56,57,67" "AMPTstring_drlt0p5#Truth AMPT (String Melt)#1" 602,860,858,419,414,434,635,629,626 "AMPT (String Melt) corr w/ HYDJET" AMPTstringClose-Hydjet_drlt0p5
# ./compile_cl "AMPTnostringClose-Hydjet_drlt0p5#" "12,13,14,23,24,34,56,57,67" "AMPTnostring_drlt0p5#Truth AMPT (No String)#1" 602,860,858,419,414,434,635,629,626 "AMPT (No String) corr w/ HYDJET" AMPTnostringClose-Hydjet_drlt0p5
