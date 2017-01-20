#!/bin/bash

bin="bin/routescheduling"

solutionfile="solution.dat"
configfile="config/grasp/config-grasp.dat"

echo "GRASP + Local Search Exchange First Improvement"
echo "---------------------------"

for locs in 8 16 32 64 128 256; do
	echo "Problem size: $locs locations"
	inputfile="instances/input${locs}locs.mh.dat"

	for repeat in $(seq 1); do
		./$bin $configfile $inputfile $solutionfile
	done
done
echo
