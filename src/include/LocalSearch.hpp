#ifndef LOCAL_SEARCH_HPP
#define LOCAL_SEARCH_HPP

#include <iostream>
#include <limits>
#include <string>
#include <ctime>

#include "Configuration.hpp"
#include "Solution.hpp"

class Change {

	public:
		int _targetId;
		int _newPrevId;
		int _newNextId;

	Change() :
		_targetId(),
		_newPrevId(),
		_newNextId()
	{
	}

	Change( int targetId, int newPrevId, int newNextId ) :
		_targetId( targetId ),
		_newPrevId( newPrevId ),
		_newNextId( newNextId )
	{
	}
};

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

		bool isEnabled()
		{
			return _enabled;
		}

		float evaluateNeighbor( Solution &solution, Change change )
		{
			float newObjFunction = std::numeric_limits<float>::infinity();
			int target = change._targetId;
			int newPrev = change._newPrevId;
			int newNext = change._newNextId;

			// Assignment to restore
			Assignment originalAssig = solution.getAssignment( target );

			// Unassign the original one
			bool feasible = solution.unassign( originalAssig );
			assert(feasible);

			// Tries to assign the new change
			Assignment newAssig( target, newPrev, newNext, 0 );
			feasible = solution.assign( newAssig );

			// If the new change is feasible, then
			// get the new objective function and
			// unassign it again
			if ( feasible ) {
				newObjFunction = solution.getObjFunction();
				feasible = solution.unassign( newAssig );
				assert(feasible);
			}

			// Before returning, the original assignment
			// must be assigned
			feasible = solution.assign( originalAssig );
			assert(feasible);

			return newObjFunction;
		}

		float evaluateNeighbor( Solution &solution, Change changes[2] )
		{
			float newObjFunction = std::numeric_limits<float>::infinity();
			bool feasible;

			int target1 = changes[0]._targetId;
			int newPrev1 = changes[0]._newPrevId;
			int newNext1 = changes[0]._newNextId;

			int target2 = changes[1]._targetId;
			int newPrev2 = changes[1]._newPrevId;
			int newNext2 = changes[1]._newNextId;

			Assignment originalAssig1 = solution.getAssignment( target1 );
			Assignment originalAssig2 = solution.getAssignment( target2 );
			feasible = solution.unassign( originalAssig1 );
			assert(feasible);
			feasible = solution.unassign( originalAssig2 );
			assert(feasible);

			Assignment newAssig1( target1, newPrev1, newNext1, 0 );
			Assignment newAssig2( target2, newPrev2, newNext2, 0 );

			feasible = solution.assign( newAssig1 );
			if ( feasible ) {
				feasible = solution.assign( newAssig2 );
				if ( feasible ) {
					newObjFunction = solution.getObjFunction();
					feasible = solution.unassign( newAssig2 );
					assert(feasible);
				}
				feasible = solution.unassign( newAssig1 );
				assert(feasible);
			}

			feasible = solution.assign( originalAssig1 );
			assert(feasible);
			feasible = solution.assign( originalAssig2 );
			assert(feasible);

			return newObjFunction;
		}

		Solution createNeighbor( Solution &solution, Change change )
		{
			Solution newSolution( solution );
			int target = change._targetId;
			int newPrev = change._newPrevId;
			int newNext = change._newNextId;

			Assignment originalAssig = newSolution.getAssignment( target );

			// Unassign the original one
			bool feasible = newSolution.unassign( originalAssig );
			assert(feasible);

			// Tries to assign the new change
			Assignment newAssig( target, newPrev, newNext, 0 );
			feasible = newSolution.assign( newAssig );
			assert(feasible);

			return newSolution;
		}

		Solution createNeighbor( Solution &solution, Change changes[2] )
		{
			Solution newSolution( solution );
			bool feasible;

			int target1 = changes[0]._targetId;
			int newPrev1 = changes[0]._newPrevId;
			int newNext1 = changes[0]._newNextId;

			int target2 = changes[1]._targetId;
			int newPrev2 = changes[1]._newPrevId;
			int newNext2 = changes[1]._newNextId;

			Assignment originalAssig1 = newSolution.getAssignment( target1 );
			Assignment originalAssig2 = newSolution.getAssignment( target2 );
			feasible = newSolution.unassign( originalAssig1 );
			assert(feasible);
			feasible = newSolution.unassign( originalAssig2 );
			assert(feasible);

			Assignment newAssig1( target1, newPrev1, newNext1, 0 );
			Assignment newAssig2( target2, newPrev2, newNext2, 0 );

			feasible = newSolution.assign( newAssig1 );
			assert(feasible);
			feasible = newSolution.assign( newAssig2 );
			assert(feasible);

			return newSolution;
		}

		Solution exploreNeighborhood( Solution &solution )
		{
			float neighborObjFunction;
			float currObjFunction = solution.getObjFunction();
			Solution bestNeighbor = solution;

			std::vector<Assignment> assigs;
			if ( _policy == "BestImprovement" )
				assigs = solution.getLocationAssignments();
			else
				assigs = solution.getLocationAssignmentsSortedByPathLength();

			if ( _neighborhoodStrategy == "Reassignment" ) {
				std::vector< std::pair<int, int> > edges =
						solution.getCurrentEdges();

				for ( Assignment &assig : assigs ) {
					for ( std::pair<int, int> &edge : edges ) {
						Change change( assig.getTargetLocation(), edge.first, edge.second );
						neighborObjFunction = evaluateNeighbor( solution, change );
						if ( currObjFunction > neighborObjFunction ) {
							bestNeighbor = createNeighbor( solution, change );
							if ( _policy == "FirstImprovement" )
								return bestNeighbor;
							currObjFunction = bestNeighbor.getObjFunction();
						}
					}
				}
			}
			else if ( _neighborhoodStrategy == "Exchange" ) {
				int numAssigs = assigs.size();

				for ( int i = 0; i < numAssigs; ++i ) {
					Assignment &assig1 = assigs[i];
					int target1 = assig1.getTargetLocation();
					int prev1 = assig1.getPrevLocation();
					int next1 = assig1.getNextLocation();

					for ( int j = numAssigs - 1; j >= 0; --j ) {
						if ( i >= j ) continue;

						Assignment &assig2 = assigs[j];
						int target2 = assig2.getTargetLocation();
						int prev2 = assig2.getPrevLocation();
						int next2 = assig2.getNextLocation();

						// Avoid this complex case
						if ( target1 == prev2 || target1 == next2 || target2 == prev1 || target2 == next1 )
							continue;

						Change changes[2];
						changes[0] = Change( target1, prev2, next2 );
						changes[1] = Change( target2, prev1, next1 );

						neighborObjFunction = evaluateNeighbor( solution, changes );
						if ( currObjFunction > neighborObjFunction ) {
							bestNeighbor = createNeighbor( solution, changes );
							if ( _policy == "FirstImprovement" )
								return bestNeighbor;
							currObjFunction = bestNeighbor.getObjFunction();
						}
					}
				}
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

#ifdef VERBOSE
			std::cout << "Local Search Performance:" << std::endl;
			std::cout << "	Number Iterations Evaluated: " << _iterations << std::endl;
			std::cout << "	Total Evaluation Time: " << _elapsedTime << std::endl;
			std::cout << "	Average Time / Iteration: " << avgEvalTimePerIteration << std::endl;
#endif
		}
};

#endif // LOCAL_SEARCH_HPP
