#ifndef SOLVER_GREEDY_HPP
#define SOLVER_GREEDY_HPP

#include "Configuration.hpp"
#include "Solution.hpp"
#include "Problem.hpp"
#include "Solver.hpp"

class SolverGreedy : public Solver {

	private:

	public:
		SolverGreedy()
		{
		}

		Solution solve( Configuration &config, Problem &problem )
		{
			return Solution( problem );
		}
};

#endif // SOLVER_GREEDY_HPP
