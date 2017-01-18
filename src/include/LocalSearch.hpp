#ifndef LOCAL_SEARCH_HPP
#define LOCAL_SEARCH_HPP

#include <iostream>
#include <string>
#include <ctime>

#include "Configuration.hpp"
#include "Solution.hpp"

class LocalSearch {

	private:
		Configuration *_config;
		Problem *_problem;

		bool _enabled;
		std::string _neighborhoodStrategy;
		std::string _policy;

		long long _iterations;
		double _elapsedTime;

	public:
		LocalSearch( Configuration &config, Problem &problem ) :
			_config( &config ),
			_problem( &problem ),
			_enabled( config.localSearch ),
			_neighborhoodStrategy( config.neighborhoodStrategy ),
			_policy( config.policy ),
			_iterations( 0 ),
			_elapsedTime( 0.0 )
		{
		}

		// TODO: LocalSearch::exploreNeighborhood
		Solution exploreNeighborhood( Solution &solution )
		{
			float currObjFunction = solution.getObjFunction();
			Solution bestNeighbor = solution;

			std::vector<Assignment> assigs;
			if ( _policy == "Best Improvement" )
				assigs = solution.getLocationAssignments();
			else
				assigs = solution.getLocationAssignmentsSortedByPathLength();

			if ( _neighborhoodStrategy == "Reassignment" ) {

				for ( Assignment &assig : assigs ) {
					
				}
			}
			else if ( _neighborhoodStrategy == "Exchange" ) {
				
			}
			else assert( false );

			return bestNeighbor;
		}

		Solution run( Solution &solution )
		{
			if ( !_enabled || !solution.isFeasible() )
				return Solution( solution );

			Solution bestSolution = solution;
			float bestObjFunction = bestSolution.getObjFunction();

			clock_t startEvalTime = clock();
			long long iterations = 0;

			bool keepIterating = true;
			while ( keepIterating ) {
				keepIterating = false;
				++iterations;

				Solution neighbor = exploreNeighborhood( bestSolution );
				float currObjFunction = neighbor.getObjFunction();
				if ( bestObjFunction > currObjFunction ) {
					bestSolution = neighbor;
					bestObjFunction = currObjFunction;
					keepIterating = true;
				}
			}

			_iterations += iterations;
			_elapsedTime += ((double) (clock() - startEvalTime) / CLOCKS_PER_SEC );

			return bestSolution;
		}

		void printPerformance()
		{
			if ( !_enabled ) return;

			double avgEvalTimePerIteration = 0.0;
			if ( _iterations != 0 )
				avgEvalTimePerIteration = 1000.0 * _elapsedTime / double(_iterations);

			std::cout << "Local Search Performance:" << std::endl;
			std::cout << "	Number Iterations Evaluated: " << _iterations << std::endl;
			std::cout << "	Total Evaluation Time: " << _elapsedTime << std::endl;
			std::cout << "	Average Time / Iteration: " << avgEvalTimePerIteration << std::endl;
		}
};

#endif // LOCAL_SEARCH_HPP
