#!/bin/sh
cd hong
for j in 1 2 3 4 5 6 7 8 9 10
do
	./scc ../graphs/${1}.bin 1 3
done
for i in 1 2 4 8 16 32 64
do
	for j in 1 2 3 4 5 6 7 8 9 10
	do
		./scc ../graphs/${1}.bin ${i} 1
	done
done
for i in 1 2 4 8 16 32 64
do
	for j in 1 2 3 4 5 6 7 8 9 10
	do
		./scc ../graphs/${1}.bin ${i} 2
	done
done
for i in 1 2 4 8 16 32 64
do
	for j in 1 2 3 4 5 6 7 8 9 10
	do
		./scc ../graphs/${1}.bin ${i} 5
	done
done
