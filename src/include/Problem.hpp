#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include "InputData.hpp"

class Problem {

	private:

	public:
		int nLocations;

	public:
		Problem( InputData &inputData ) :
			nLocations( inputData.nLocations )
		{
		}

		bool checkInstance()
		{
			return true;
		}
};

#endif // PROBLEM_HPP
