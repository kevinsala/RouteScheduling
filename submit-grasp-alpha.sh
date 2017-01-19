#!/bin/bash

configdir="config/grasp/alpha"
bin="bin/routescheduling"

solutionfile="solution.dat"

alphas="0.0 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 1.0"
for alpha in $alphas; do
	configfile="${configdir}/config.grasp.alpha.${alpha}.dat"
	echo "GRASP Alpha = $alpha"
	echo "---------------------------"

	for locs in 8 16 32 64; do
		echo "Problem size: $locs locations"
		inputfile="instances/input${locs}locs.mh.dat"
		./$bin $configfile $inputfile $solutionfile
	done
	echo
done
