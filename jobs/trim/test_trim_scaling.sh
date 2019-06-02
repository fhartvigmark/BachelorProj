#!/bin/sh
cd src
for i in 1 2 4 8 16 24 32
do
	for j in 1 2 3 4 5 6 7 8 9 10
	do
		./main -g=../graphs/${1}.edges -w=${i} -time=3 -a=3 -suffix=Trim_1_Col_C${i}_iteration${j} -m=1 -t=1
	done
	cat ${1}Trim_1_Col_C${i}_iteration*.time > ${1}Trim_1_Col_C${i}.time
	rm ${1}Trim_1_Col_C${i}_iteration*.time
done
for i in 1 2 4 8 16 24 32
do
	for j in 1 2 3 4 5 6 7 8 9 10
	do
		./main -g=../graphs/${1}.edges -w=${i} -time=3 -a=3 -suffix=Trim_2_Col_C${i}_iteration${j} -m=1 -t=2
	done
	cat ${1}Trim_2_Col_C${i}_iteration*.time > ${1}Trim_2_Col_C${i}.time
	rm ${1}Trim_2_Col_C${i}_iteration*.time
done
for i in 1 2 4 8 16 24 32
do
	for j in 1 2 3 4 5 6 7 8 9 10
	do
		./main -g=../graphs/${1}.edges -w=${i} -time=3 -a=3 -suffix=Trim_3_Col_C${i}_iteration${j} -m=1 -t=3
	done
	cat ${1}Trim_3_Col_C${i}_iteration*.time > ${1}Trim_3_Col_C${i}.time
	rm ${1}Trim_3_Col_C${i}_iteration*.time
done
