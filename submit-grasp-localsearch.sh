#!/bin/bash

configdir="config/grasp/localsearch"
bin="bin/routescheduling"

solutionfile="solution.dat"

strategies="reassign exchange"
policies="best first"
for strategy in $strategies; do
	for policy in $policies; do
		configfile="${configdir}/config.grasp.localsearch.${strategy}.${policy}.dat"
		echo "GRASP Local Search $strategy $policy improvement"
		echo "---------------------------"

		for locs in 8 16 32 64; do
			echo "Problem size: $locs locations"
			inputfile="instances/input${locs}locs.mh.dat"
			./$bin $configfile $inputfile $solutionfile
		done
	done
	echo
done
