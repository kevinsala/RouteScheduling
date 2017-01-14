#ifndef SOLVER_BRKGA_HPP
#define SOLVER_BRKGA_HPP

#include <cassert>
#include <ctime>

#include "Configuration.hpp"
#include "Solution.hpp"
#include "Problem.hpp"
#include "Decoder.hpp"
#include "MTRand.hpp"
#include "Solver.hpp"
#include "BRKGA.hpp"

class SolverBRKGA : public Solver {

	private:

	public:
		SolverBRKGA()
		{
		}

		Solution solve( Configuration &config, Problem &problem, Decoder &decoder )
		{
			clock_t startTime = clock();

			const unsigned n = decoder.getNumGenes();		// size of chromosomes
			const unsigned p = decoder.getNumIndividuals();	// size of population
			const double pe = config.pElites;		// fraction of population to be the elite-set
			const double pm = config.pMutants;		// fraction of population to be replaced by mutants
			const double rhoe = config.pInheritanceElite;	// probability that offspring inherit an allele from elite parent

			MTRand rng( (int) startTime );

			InfoBRKGA info( problem );
			info.bestSolution.makeInfeasible();
			decoder.setInfoPtr( &info );

			// initialize the BRKGA-based heuristic
			BRKGA<Decoder, MTRand> algorithm( n, p, pe, pm, rhoe, decoder, rng, 1, 1 );

			long long generation = 0;		// current generation
			const unsigned X_INTVL = 100;	// exchange best individuals at every 100 generations
			const unsigned X_NUMBER = 2;	// exchange top 2 best

			while ( ((double)(clock() - startTime)) / CLOCKS_PER_SEC < config.maxExecTime ) {
				algorithm.evolve();	// evolve the population for one generation
				
				if ( ++generation % X_INTVL == 0 )
					algorithm.exchangeElite( X_NUMBER );	// exchange top individuals
			}

			assert(info.bestSolution.getObjFunction() == algorithm.getBestFitness());

			double avgDecodingTimePerIndividual = 0;
			if ( info.totalDecodedIndividuals != 0 )
				avgDecodingTimePerIndividual = 1000.0 * info.totalDecodingTime / double(info.totalDecodedIndividuals);

			std::cout << "BRKGA Individual Decoder Performance:" << std::endl;
			std::cout << "	Number Individuals Decoded: " << info.totalDecodedIndividuals << std::endl;
			std::cout << "	Total Decoding Time: " << info.totalDecodingTime << "s" << std::endl;
			std::cout << "	Average Time / Individual: " << avgDecodingTimePerIndividual << "ms" << std::endl;

			return info.bestSolution;
		}
};

#endif // SOLVER_BRKGA_HPP
