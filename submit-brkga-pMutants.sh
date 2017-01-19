#!/bin/bash

configdir="config/brkga/pMutants"
bin="bin/routescheduling"

solutionfile="solution.dat"

pMutantsValues="0.1 0.15 0.2 0.4"
for pMutants in $pMutantsValues; do
	configfile="${configdir}/config.brkga.pMutants.${pMutants}.dat"
	echo "BRKGA pMutants = $pMutants"
	echo "---------------------------"

	for locs in 8 16 32 64; do
		echo "Problem size: $locs locations"
		inputfile="instances/input${locs}locs.mh.dat"
		./$bin $configfile $inputfile $solutionfile
	done
	echo
done
