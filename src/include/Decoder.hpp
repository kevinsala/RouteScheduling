#ifndef DECODER_HPP
#define DECODER_HPP

#include <vector>
#include <cmath>
#include <ctime>

#include "Solution.hpp"

class InfoBRKGA
{
	public:
		Solution bestSolution;
		double totalDecodingTime;
		long long totalDecodedIndividuals;
	
		InfoBRKGA( Problem &problem ) :
			bestSolution( problem ),
			totalDecodingTime( 0 ),
			totalDecodedIndividuals( 0 )
		{
		}
};

class Decoder {

	private:
		Configuration *_config;
		Problem *_problem;
		InfoBRKGA *_info;

		typedef std::pair<Location, double> loc_and_gene_t;

	public:
		Decoder( Configuration &config, Problem &problem ) :
			_config( &config ),
			_problem( &problem ),
			_info( nullptr )
		{
		}

		int getNumGenes() const
		{
			return _problem->getNLocations() - 1;
		}

		int getNumIndividuals() const
		{
			return (int) std::round( (_problem->getNLocations() - 1) * _config->fIndividuals );
		}

		void setInfoPtr( InfoBRKGA * info )
		{
			_info = info;
		}

		double decode( const std::vector<double> &chromosome ) const
		{
			assert( _info != nullptr );

			clock_t startTime = clock();

			Problem &problem = *_problem;
			int numGenes = getNumGenes();
			std::vector<Location> &locations =
				problem.getTaskLocationsSortedByMinWindowDescending();

			std::vector< loc_and_gene_t > locs_and_genes( numGenes );
			for ( int i = 0; i < numGenes; ++i )
				locs_and_genes[i] = loc_and_gene_t( locations[i], chromosome[i] );

			std::sort( locs_and_genes.begin(), locs_and_genes.end(), compareByLocsAndGenesDescending );
			Solution solution( problem );

			for ( loc_and_gene_t &loc_and_gene : locs_and_genes ) {
				int locationId = loc_and_gene.first.getId();

				Assignment assig = solution.findBestFeasibleAssignment( locationId );
				if ( assig.getTargetLocation() < 0 ) {
					solution.makeInfeasible();
					break;
				}

				solution.assign( assig );
			}

			if ( _info->bestSolution.getObjFunction() > solution.getObjFunction() ) {
				// Although this method is defined as const, we can update
				// the best solution of BRKGA through a pointer
				_info->bestSolution = solution;
			}

			_info->totalDecodedIndividuals += 1;
			_info->totalDecodingTime += ((double)(clock() - startTime)) / CLOCKS_PER_SEC;

			return solution.getObjFunction();
		}

		static bool compareByLocsAndGenesDescending( const loc_and_gene_t &first, const loc_and_gene_t &second )
		{
			return (first.first.getMinWindow() * first.second) > (second.first.getMinWindow() * second.second);
		}
};

#endif // DECODER_HPP
