#!/bin/sh
for i in 1 2 4 8 16
do
	for j in 1 2 3 4 5 6 7 8 9 10
	do
		./pivot_extract SlashdotPivotRandom_C${i}_iteration${j}.debug PivotRandom_C${i}_iteration${j}.out
	done
done
for i in 1 2 4 8 16
do
	for j in 1 2 3 4 5 6 7 8 9 10
	do
		./pivot_extract SlashdotPivotRandWalk_C${i}_iteration${j}.debug PivotRandWalk_C${i}_iteration${j}.out
	done
done
for i in 1 2 4 8 16
do
	for j in 1 2 3 4 5 6 7 8 9 10
	do
		./pivot_extract SlashdotPivotMaxDeg_C${i}_iteration${j}.debug PivotMaxDeg_C${i}_iteration${j}.out
	done
done
for i in 1 2 4 8 16
do
	for j in 1 2 3 4 5 6 7 8 9 10
	do
		./pivot_extract SlashdotPivotMaxDeg_Col_C${i}_iteration${j}.debug PivotMaxDegCol_C${i}_iteration${j}.out
	done
done
for i in 1 2 4 8 16
do
	for j in 1 2 3 4 5 6 7 8 9 10
	do
		./pivot_extract SlashdotPivotMaxDeg_precomp_C${i}_iteration${j}.debug PivotMaxDegPre_C${i}_iteration${j}.out
	done
done
for i in 1 2 4 8 16
do
	for j in 1 2 3 4 5 6 7 8 9 10
	do
		./pivot_extract SlashdotPivotRandWalk2_C${i}_iteration${j}.debug PivotRandWalk2_C${i}_iteration${j}.out
	done
done