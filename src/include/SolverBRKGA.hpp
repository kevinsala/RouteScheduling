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

		// TODO: SolverBRKGA::solve
		Solution solve( Configuration &config, Problem &problem, Decoder &decoder )
		{
			return Solution( problem );
		}
};

#endif // SOLVER_BRKGA_HPP
