#ifndef SOLVER_BRKGA_HPP
#define SOLVER_BRKGA_HPP

#include "Configuration.hpp"
#include "Solution.hpp"
#include "Problem.hpp"
#include "Decoder.hpp"
#include "Solver.hpp"

class SolverBRKGA : public Solver {

	private:

	public:
		SolverBRKGA()
		{
		}

		void solve( Configuration &config, Problem &problem, Solution &solution, Decoder &decoder )
		{

		}
};

#endif // SOLVER_BRKGA_HPP
