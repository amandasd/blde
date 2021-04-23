#!/bin/bash

home="$(pwd)"
seq="$home/sequential"

dimL=("0" "2" "5" "10")
dimF=("0" "3" "5" "10")
popL=("0" "32" "64" "128" "256" "512")
popF=("0" "32" "64" "128" "256" "512")
genF=("0" "50" "100" "200" "300" "500")
func=("0" "1001" "1002" "1003" "1004" "1005" "1006" "1007" "1008")

cd $seq
g++ -o3 -o main blde.cpp funcoes.cpp

#SEQUENTIAL
for s in $(seq 1 1); do

   for f in $(seq 2 2); do
   
      for d in $(seq 2 2); do
   
         for pL in $(seq 2 5); do
   
            for pF in $(seq 2 5); do
   
               for gF in $(seq 2 5); do
   
                  cd $seq
		  echo $s ${popL[$pL]} ${popF[$pF]} ${genF[$gF]} ${dimF[$d]} ${dimL[$d]}
                  ./main -popL ${popL[$pL]} -popF ${popF[$pF]} -genF ${genF[$gF]} -genL 10000 -dimF ${dimF[$d]} -dimL ${dimL[$d]} -func ${func[$f]} -v 3 -F 0.8 -CR 0.9 -seed $(date +'%N') >> $home/output/seq_${func[$f]}_${popL[$pL]}_${popF[$pF]}_${genF[$gF]}_${dimL[$d]}x${dimF[$d]}
                  exit
   
               done
   
            done
   
         done
   
      done
   
   done

done
