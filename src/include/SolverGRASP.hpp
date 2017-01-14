#ifndef SOLVER_GRASP_HPP
#define SOLVER_GRASP_HPP

#include <cstdlib>
#include <ctime>
#include <list>

#include "Configuration.hpp"
#include "LocalSearch.hpp"
#include "Assignment.hpp"
#include "Location.hpp"
#include "Solution.hpp"
#include "Problem.hpp"
#include "Solver.hpp"

class SolverGRASP : public Solver {

	private:

	public:
		SolverGRASP()
		{
		}

		Assignment *selectCandidate( Configuration &config, std::list<Assignment> &candidateList )
		{
			if ( candidateList.empty() ) return nullptr;

			// Sort the candidate list by the objective function in ascending order
			candidateList.sort( Assignment::compareByObjFunctionAscending );

			// Compute the minimum, maximum and the boundary
			float minObjFunction = candidateList.front().getObjFunction();
			float maxObjFunction = candidateList.back().getObjFunction();
			float boundaryObjFunction = minObjFunction + (maxObjFunction - minObjFunction) * config.alpha;

			// Compute the range of the RCL
			int maxIndex = 0;
			std::list<Assignment>::iterator it;
			for ( it = candidateList.begin(); it != candidateList.end(); ++it ) {
				Assignment &candidate = *it;
				if (candidate.getObjFunction() > boundaryObjFunction) break;
				++maxIndex;
			}
			assert( maxIndex != 0 );

			// Choose one candidate of the RCL at random
			int randomChoice = rand() % maxIndex;
			it = candidateList.begin();
			for ( int i = 0; i < randomChoice; ++i ) ++it;
			return &(*it);
		}

		Solution greedyRandomizedConstruction( Configuration &config, Problem &problem, double &itElapsedEvalTime, long long &itEvalCandidates )
		{
			Solution solution( problem );
			itElapsedEvalTime = 0.0;
			itEvalCandidates = 0;

			std::vector<Location> &locations
				= problem.getTaskLocationsSortedByMinWindowDescending();

			std::list<Assignment> candidateList;
			double elapsedEvalTime = 0.0;
			long long evalCandidates = 0;

			std::list<Location>::iterator locIt;
			for ( Location &location : locations ) {
				std::list<Assignment> candidateList =
					solution.findFeasibleAssignments( location.getId(), elapsedEvalTime, evalCandidates );
				itElapsedEvalTime += elapsedEvalTime;
				itEvalCandidates += evalCandidates;

				if ( candidateList.empty() ) {
					solution.makeInfeasible();
					break;
				}

				Assignment *selectedCandidate = selectCandidate( config, candidateList );
				assert(selectedCandidate != nullptr);

				solution.assign( *selectedCandidate );
			}

			return solution;
		}

		Solution solve( Configuration &config, Problem &problem )
		{
			Solution bestSolution( problem );
			bestSolution.makeInfeasible();
			float bestObjFunction = bestSolution.getObjFunction();

			std::clock_t startTime = clock();

			double totalElapsedEvalTime = 0.0;
			double itElapsedEvalTime = 0.0;
			long long totalEvalCandidates = 0;
			long long itEvalCandidates = 0;
			long long iteration = 0;

			LocalSearch localSearch( config );

			while ( ((double)(clock() - startTime)) / CLOCKS_PER_SEC < config.maxExecTime ) {
				iteration += 1;

				float originalAlpha = config.alpha;
				if ( iteration == 1 ) config.alpha = 0;

				Solution solution = greedyRandomizedConstruction( config, problem, itElapsedEvalTime, itEvalCandidates );
				totalElapsedEvalTime += itElapsedEvalTime;
				totalEvalCandidates += itEvalCandidates;

				if ( iteration == 1 ) config.alpha = originalAlpha;
				if ( !solution.isFeasible() ) continue;

				solution = localSearch.run( solution );

				float solutionObjFunction = solution.getObjFunction();
				if ( solutionObjFunction < bestObjFunction ) {
					bestSolution = solution;
					bestObjFunction = solutionObjFunction;
				}
			}

			double avgEvalTimePerCandidate = 0;
			if ( totalEvalCandidates != 0 )
				avgEvalTimePerCandidate = 1000.0 * totalElapsedEvalTime / double(totalEvalCandidates);

			std::cout << "GRASP Candidate Evaluation Performance:" << std::endl;
			std::cout << "	Number Candidates Evaluated: " << totalEvalCandidates << std::endl;
			std::cout << "	Total Evaluated Time: " << totalElapsedEvalTime << "s" << std::endl;
			std::cout << "	Average Time / Candidate: " << avgEvalTimePerCandidate << "ms" << std::endl;

			localSearch.printPerformance();

			return bestSolution;
		}
};

#endif // SOLVER_GRASP_HPP
