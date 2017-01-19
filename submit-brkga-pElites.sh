#!/bin/bash

configdir="config/brkga/pElites"
bin="bin/routescheduling"

solutionfile="solution.dat"

pElitesValues="0.2 0.4 0.6"
for pElites in $pElitesValues; do
	configfile="${configdir}/config.brkga.pElites.${pElites}.dat"
	echo "BRKGA pElites = $pElites"
	echo "---------------------------"

	for locs in 8 16 32 64; do
		echo "Problem size: $locs locations"
		inputfile="instances/input${locs}locs.mh.dat"
		./$bin $configfile $inputfile $solutionfile
	done
	echo
done
