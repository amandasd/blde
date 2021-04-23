#!/bin/bash

home="$(pwd)"
seq="$home/sequential"

dimL=("0" "2" "5" "10")
dimF=("0" "3" "5" "10")
popL=("0" "32" "64" "128")
popF=("0" "32" "128")
genF=("0" "32" "128")
func=("0" "1001" "1002" "1003" "1004" "1005" "1006" "1007" "1008")

cd $seq
g++ -o3 -o main blde.cpp funcoes.cpp

#SEQUENTIAL
for s in $(seq 1 1); do

   for f in $(seq 2 2); do
   
      for d in $(seq 2 2); do
   
         for pL in $(seq 1 2); do
   
            for pF in $(seq 1 2); do
   
               for gF in $(seq 1 2); do
   
                  cd $seq
                  ./main -popL ${popL[$pL]} -popF ${popF[$pF]} -genF ${genF[$gF]} -genL 100 -dimF ${dimF[$d]} -dimL ${dimL[$d]} -func ${func[$f]} -v 3 -F 0.8 -CR 0.9 -seed $(date +'%N') >> $home/output/seq_${func[$f]}_${popL[$pL]}_${popF[$pF]}_${genF[$gF]}_${dimL[$d]}x${dimF[$d]}
   
               done
   
            done
   
         done
   
      done
   
   done

done


