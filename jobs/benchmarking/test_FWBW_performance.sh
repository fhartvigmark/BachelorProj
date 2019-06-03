#!/bin/sh
cd src

for j in 1 2 3 4 5 6 7 8 9 10
do
	./main -g=../graphs/${1}.edges -w=${i} -p=3 -time=3 -suffix=Baseline_C${i}_iteration${j} -m=0 -t=3 -cutoff=5 -steps=${2}
done
cat ${1}Baselin_C${i}_iteration*.time > ${1}Baseline_C${i}.time
rm ${1}Baseline_C${i}_iteration*.time
for i in 1 2 4 8 16 24 32 48
do
	for j in 1 2 3 4 5 6 7 8 9 10
	do
		./main -g=../graphs/${1}.edges -w=${i} -p=3 -time=3 -suffix=ParFWBW_C${i}_iteration${j} -m=1 -t=3 -cutoff=5 -steps=${2}
	done
	cat ${1}ParFWBW_C${i}_iteration*.time > ${1}ParFWBW_C${i}.time
	rm ${1}ParFWBW_C${i}_iteration*.time
done
for i in 1 2 4 8 16 24 32 48
do
	for j in 1 2 3 4 5 6 7 8 9 10
	do
		./main -g=../graphs/${1}.edges -w=${i} -p=3 -time=3 -suffix=RecFWBW_C${i}_iteration${j} -m=2 -t=3 -cutoff=5 -steps=${2}
	done
	cat ${1}RecFWBW_C${i}_iteration*.time > ${1}RecFWBW_C${i}.time
	rm ${1}RecFWBW_C${i}_iteration*.time
done
