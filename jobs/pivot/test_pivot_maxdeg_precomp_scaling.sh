#!/bin/sh
cd src 
for i in 1 2 4 8 16
do
	for j in 1 2 3 4 5 6 7 8 9 10
	do
		./main -g=../graphs/${1}.edges -p=4 -w=${i} -time=3 -a=3 -suffix=PivotMaxDeg_precomp_C${i}_iteration${j} -m=1 -t=${2}
	done
	cat ${1}PivotMaxDeg_precomp_C${i}_iteration*.time > ${1}PivotMaxDeg_precomp_C${i}.time
	rm ${1}PivotMaxDeg_precomp_C${i}_iteration*.time
done
