#!/bin/sh
cd src 
for i in 1 2 4 8 16
do
	for j in 1 2 3 4 5 6 7 8 9 10
	do
		./main -g=Slashdot0902.txt -p=3 -w=${i} -time=3 -a=3 -out= -suffix=PivotRandWalk_C${i}_iteration${j} -m=1
	done
	cat Slashdot0902PivotRandWalk_C${i}_iteration*.time > Slashdot0902PivotRandWalk_C${i}.time
	rm Slashdot0902PivotRandWalk_C${i}_iteration*.time
done
