#ifndef LOCAL_SEARCH_HPP
#define LOCAL_SEARCH_HPP

#include "Configuration.hpp"
#include "Solution.hpp"

// TODO: LocalSearch class
class LocalSearch {

	private:
		Configuration *_config;

	public:
		LocalSearch( Configuration &config ) :
			_config( &config )
		{
		}

		// TODO: LocalSearch::run
		Solution run( Solution &solution )
		{
			return Solution( solution.getProblem() );
		}

		// TODO: LocalSearch::printPerformance
		void printPerformance()
		{

		}
};

#endif // LOCAL_SEARCH_HPP
