#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "Utils.hpp"

class Solution {

	private:
		bool _empty;
		bool _feasible;

		Problem *_problem;

		int nCars;
		float objFunction;
		matrix<bool> travel;
		std::vector<float> arrive;
		std::vector<int> value;

	public:

		Solution( Problem &problem ) :
			_empty( false ),
			_feasible( false ),
			_problem( &problem ),
			objFunction( 0.0 ),
			nCars( 0 ),
			travel( problem.nLocations, std::vector<bool>( problem.nLocations, false ) ),
			arrive( problem.nLocations, 0.0 ),
			value( problem.nLocations, 0 )
		{
		}

		void makeEmpty()
		{
			_empty = true;
		}

		void makeNotEmpty()
		{
			_empty = false;
		}

		void makeFeasible()
		{
			_feasible = true;
		}

		void makeInfeasible()
		{
			_feasible = false;
		}

		void saveToFile( std::string solutionFile )
		{
			std::ofstream out;
			out.open( solutionFile );

			if ( _feasible ) {
				out << "objFunction = " << objFunction << std::endl;
				out << "nCars = " << nCars << std::endl;

				int nLocations = _problem->nLocations;
				out << "travel = [" << std::endl;
				for ( int i = 0; i < nLocations; ++i ) {
					out << "[ ";
					for ( int j = 0; j < nLocations; ++j ) {
						out << travel[i][j] << " ";
					}
					out << "]" << std::endl;
				}
				out << "]" << std::endl;

				out << "arrive = [ ";
				for ( int i = 0; i < nLocations; ++i )
					out << arrive[i] << " ";
				out << "]" << std::endl;

				out << "value = [ ";
				for ( int i = 0; i < nLocations; ++i )
					out << value[i] << " ";
				out << "]" << std::endl;
			}
			else out << "This instance is infeasible" << std::endl;

			out.close();
		}
};

#endif // SOLUTION_HPP
