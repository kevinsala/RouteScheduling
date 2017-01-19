#!/bin/bash

configdir="config/brkga/fIndividuals"
bin="bin/routescheduling"

solutionfile="solution.dat"

fIndividualsValues="1.0 1.5 2.0"
for fIndividuals in $fIndividualsValues; do
	configfile="${configdir}/config.brkga.fIndividuals.${fIndividuals}.dat"
	echo "BRKGA fIndividuals = $fIndividuals"
	echo "---------------------------"

	for locs in 8 16 32 64; do
		echo "Problem size: $locs locations"
		inputfile="instances/input${locs}locs.mh.dat"
		./$bin $configfile $inputfile $solutionfile
	done
	echo
done
