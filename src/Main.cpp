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

	if ( argc > 1 ) {
		configFilename = argv[1];
	}
	else configFilename = "config.dat";

	Configuration config;
	config.load( configFilename );

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
