#!/bin/sh
for i in 1 2 4 8 16
do
	for j in 1 2 3 4 5 6 7 8 9 10
	do
		./scc_checker SlashdotReal.scc Slashdot0902PivotRandWalk_C${i}_iteration${j}.scc s
	done
done
