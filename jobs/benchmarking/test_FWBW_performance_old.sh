#!/bin/sh
cd src
export OMP_DISPLAY_ENV=true
export OMP_CANCELLATION=true
export OMP_PROC_BIND=close
for i in 1 2 4 8 16 32 64
do
	for j in 1 2 3 4 5
	do
		./main -g=../graphs/${1}.edges -w=${i} -p=4 -time=3 -suffix=Baseline_C${i}_iteration${j} -m=0 -t=3 
	done
	cat ${1}Baseline_C${i}_iteration*.time > ${1}Baseline${i}.time
	rm ${1}Baseline_C${i}_iteration*.time
done
for i in 1 2 4 8 16 32 64
do
	for j in 1 2 3 4 5
	do
		./main -g=../graphs/${1}.edges -w=${i} -p=4 -time=3 -suffix=ParFWBW_C${i}_iteration${j} -m=1 -t=3
	done
	cat ${1}ParFWBW_C${i}_iteration*.time > ${1}ParFWBW_C${i}.time
	rm ${1}ParFWBW_C${i}_iteration*.time
done
for i in 1 2 4 8 16 32 64
do
	for j in 1 2 3 4 5
	do
		./main -g=../graphs/${1}.edges -w=${i} -p=4 -time=3 -suffix=RecFWBW_C${i}_iteration${j} -m=2 -t=3
	done
	cat ${1}RecFWBW_C${i}_iteration*.time > ${1}RecFWBW_C${i}.time
	rm ${1}RecFWBW_C${i}_iteration*.time
done
