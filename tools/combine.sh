#!/bin/sh
paste PivotRandom_C*_iteration*.out > cPivotRandom.out
paste PivotRandWalk_C*_iteration*.out > cPivotRandWalk.out
paste PivotMaxDeg_C*_iteration*.out > cPivotMaxDeg.out
paste PivotMaxDegCol_C*_iteration*.out > cPivotMaxDegCol.out
paste PivotMaxDegPre_C*_iteration*.out > cPivotMaxDegPre.out
