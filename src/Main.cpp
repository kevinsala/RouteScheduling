#include <iostream>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "include/Configuration.hpp"
#include "include/InputData.hpp"
#include "include/Problem.hpp"
#include "include/Solution.hpp"
#include "include/SolverGreedy.hpp"
#include "include/SolverGRASP.hpp"
#include "include/SolverBRKGA.hpp"
#include "include/Decoder.hpp"

int main(int argc, char * argv[])
{
	std::string configFilename;
	std::string inputFilename;
	std::string solutionFilename;

	if ( argc != 1 && argc != 4 ) {
		std::cout << "USAGE: " << argv[0] << " [configfile inputfile solutionfile]" << std::endl;
		exit( 1 );
	}

	if ( argc > 1 ) {
		configFilename = argv[1];
		inputFilename = argv[2];
		solutionFilename = argv[3];
	}
	else {
		configFilename = "config.dat";
		inputFilename = "input.dat";
		solutionFilename = "solution.dat";
	}

	Configuration config;
	config.load( configFilename, inputFilename, solutionFilename );

	InputData inputData;
	inputData.load( config.inputDataFile );

	Problem problem( inputData );
	Solution solution( problem );

	if ( problem.checkFeasibility() ) {
		if ( config.solver == "Greedy") {
			SolverGreedy solver;
			solution = solver.solve( config, problem );
		}
		else if (config.solver == "GRASP") {
			SolverGRASP solver;
			solution = solver.solve( config, problem );
		}
		else if (config.solver == "BRKGA") {
			SolverBRKGA solver;
			Decoder decoder( config, problem );
			solution = solver.solve( config, problem, decoder );
		}
	}
	else {
		std::cerr << "This instance is infeasible" << std::endl;
		solution.makeInfeasible();
	}

	solution.saveToFile( config.solutionFile );
}
