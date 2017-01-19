#!/bin/bash

configdir="config/brkga/pInheritanceElite"
bin="bin/routescheduling"

solutionfile="solution.dat"

pInheritanceEliteValues="0.5 0.6 0.7 0.8 0.9 1.0"
for pInheritanceElite in $pInheritanceEliteValues; do
	configfile="${configdir}/config.brkga.pInheritanceElite.${pInheritanceElite}.dat"
	echo "BRKGA pInheritanceElite = $pInheritanceElite"
	echo "---------------------------"

	for locs in 8 16 32 64; do
		echo "Problem size: $locs locations"
		inputfile="instances/input${locs}locs.mh.dat"
		./$bin $configfile $inputfile $solutionfile
	done
	echo
done
