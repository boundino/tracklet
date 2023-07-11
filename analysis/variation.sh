#!/bin/bash

asels=(
    "(1),"
    # "(vz[1]<-10&&vz[1]>-15),vz0"
    # "(vz[1]<-5&&vz[1]>-10),vz1"
    # "(vz[1]<0&&vz[1]>-5),vz2"
    # "(vz[1]>0&&vz[1]<5),vz3"
    # "(vz[1]>5&&vz[1]<10),vz4"
    # "(vz[1]>10&&vz[1]<15),vz5"
)
maxdr2s=(
    "0.25 drlt0p5"
    # "0.16 drlt0p4"
    # "0.36 drlt0p6"
)
multhandles=(
    "0,"
    "1,nhit2"
)
ctable=0

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

            # echo $multhandle
            # echo $tagmult
            # echo $defmult
            
            [[ $(($defasel+$defdr2+$defmult)) -eq 2 ]] || continue
            
            tagver="v1"
            [[ $defasel -eq 1 ]] || tagver=$tagver"-"$tagasel    
            [[ $defdr2 -eq 1 ]] || tagver=$tagver"-"${vmaxdr2[1]}
            [[ $defmult -eq 1 ]] || tagver=$tagver"-"$tagmult    

            echo $tagver

            [[ $defasel -eq 0 ]] && ./rrvar.sh ${1:-0011} 00 $tagver "$asel" $maxdr2 $multhandle $ctable
            [[ $defdr2 -eq 0 ]] && ./rrvar.sh ${1:-1011} 10 $tagver "$asel" $maxdr2 $multhandle $ctable
            [[ $defmult -eq 0 ]] && ./rrvar.sh ${1:-1111} 11 $tagver "$asel" $maxdr2 $multhandle $ctable
        done        
    done
done
