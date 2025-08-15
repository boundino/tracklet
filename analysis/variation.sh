#!/bin/bash

make reap_results || exit 1
make merge_monde || exit 1

asels=(
    "(1),"
    
    "(vz[1]<-5&&vz[1]>-10),vz1"
    "(vz[1]<0&&vz[1]>-5),vz2"
    "(vz[1]>0&&vz[1]<5),vz3"
    "(vz[1]>5&&vz[1]<10),vz4"

    "(lumi>=52&&lumi<=99),lumi0"
    "(lumi>=100&&lumi<=163),lumi1"
    "(lumi>=52&&lumi<=74),lumi2"
    "(lumi>=75&&lumi<=99),lumi3"
    "(lumi>=100&&lumi<=129),lumi4"
    "(lumi>=130&&lumi<=163),lumi5"
)
maxdr2s=(
    "0.25 drlt0p5"
    "0.16 drlt0p4"
    "0.36 drlt0p6"
)
multhandles=(
    "0,"
    "1,nhit2"
)
ctableindexs=(
    "0,"
    "1,hfeffup"
    "2,hfeffdown"
)
hfsels=(
    "(nhfp > 1 && nhfn > 1),"
    "(nhfp_low > 2 && nhfn_low > 2),evtsel1"
    "(nhfp_high > 1 && nhfn_high > 1),evtsel2"
)


for vasels in "${asels[@]}" ; do

    IFS=',' ; vasel=($vasels) ; unset IFS ;
    asel=${vasel[0]}
    tagasel=${vasel[1]}
    [[ x$tagasel == x ]] && defasel=1 || defasel=0
    
    for maxdr2 in "${maxdr2s[@]}" ; do
        vmaxdr2=($maxdr2)
        [[ ${vmaxdr2[0]} == 0.25 ]] && defdr2=1 || defdr2=0

        for mults in "${multhandles[@]}" ; do

            IFS=',' ; vmult=($mults) ; unset IFS ;
            multhandle=${vmult[0]}
            tagmult=${vmult[1]}
            [[ x$tagmult == x ]] && defmult=1 || defmult=0

            for ctables in "${ctableindexs[@]}" ; do

                IFS=',' ; vctable=($ctables) ; unset IFS ;
                ctable=${vctable[0]}
                tagctable=${vctable[1]}
                [[ x$tagctable == x ]] && defctable=1 || defctable=0

                for vhfsels in "${hfsels[@]}" ; do
                    
                    IFS=',' ; vhfsel=($vhfsels) ; unset IFS ;
                    hfsel=${vhfsel[0]}
                    taghfsel=${vhfsel[1]}
                    [[ x$taghfsel == x ]] && defhfsel=1 || defhfsel=0
    
                    [[ $(($defasel+$defdr2+$defmult+$defctable+$defhfsel)) -eq 4 ]] || continue
                
                    tagver="v1"
                    [[ $defasel -eq 1 ]] || tagver=$tagver"-"$tagasel
                    [[ $defdr2 -eq 1 ]] || tagver=$tagver"-"${vmaxdr2[1]}
                    [[ $defmult -eq 1 ]] || tagver=$tagver"-"$tagmult
                    [[ $defctable -eq 1 ]] || tagver=$tagver"-"$tagctable
                    [[ $defhfsel -eq 1 ]] || tagver=$tagver"-"$taghfsel
                    
                    echo "--> "$tagver
                    echo "asel: "$asel
                    echo "maxdr2: "$maxdr2
                    echo "ctable: "$ctable
                    echo "multhandle: "$multhandle
                    echo "hfsel: "$hfsel
                    echo
                    
                    [[ $defasel -eq 0 ]] && ./rrvar.sh ${1:-0011} 00 $tagver "$asel" $maxdr2 $multhandle $ctable "$hfsel" 1
                    [[ $defdr2 -eq 0 ]] && ./rrvar.sh ${1:-1011} 10 $tagver "$asel" $maxdr2 $multhandle $ctable "$hfsel" 1
                    [[ $defmult -eq 0 ]] && ./rrvar.sh ${1:-1111} 11 $tagver "$asel" $maxdr2 $multhandle $ctable "$hfsel" 1
                    [[ $defctable -eq 0 ]] && ./rrvar.sh ${1:-0011} 00 $tagver "$asel" $maxdr2 $multhandle $ctable "$hfsel" 1
                    [[ $defhfsel -eq 0 ]] && ./rrvar.sh ${1:-1111} 11 $tagver "$asel" $maxdr2 $multhandle $ctable "$hfsel" 1
                done
            done
        done        
    done
done
