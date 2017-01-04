#ifndef INPUT_DATA_HPP
#define INPUT_DATA_HPP

#include <cassert>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

class InputData {

	private:

		int nLocations;
		int startLocation;

		float *distance;
		float *task;
		float *minWindow;
		float *maxWindow;

	public:
		InputData()
		{
		}

		void load( std::string inputDataFile )
		{
			std::string token;
  			std::ifstream in( inputDataFile );

			if ( !in ) {
    			std::cerr << "Cannot open the input data file\n" << std::endl;
    			return;
  			}

			// Get the number of locations
			in >> token; assert(token == "nLocations");
			in >> token; assert(token == "=");
			in >> nLocations;

			// Get the starting location
			in >> token; assert(token == "startLocation");
			in >> token; assert(token == "=");
			in >> startLocation;

			// Allocate memory to store the input
			distance = (float *) malloc( nLocations * nLocations * sizeof(float) );
			task = (float *) malloc( nLocations * sizeof(float) );
			minWindow = (float *) malloc( nLocations * sizeof(float) );
			maxWindow = (float *) malloc( nLocations * sizeof(float) );

			// Get the matrix of distances
			in >> token; assert(token == "distance");
			in >> token; assert(token == "=");
			in >> token; assert(token == "[");
			for (int i = 0; i < nLocations; i++) {
				in >> token; assert(token == "[");
				for (int j = 0; j < nLocations; j++) {
					in >> distance[i * nLocations + j];
				}
				in >> token; assert(token == "]");
			}
			in >> token; assert(token == "]");

			// Get the array of task duration
			in >> token; assert(token == "task");
			in >> token; assert(token == "=");
			in >> token; assert(token == "[");
			for (int i = 0; i < nLocations; i++) {
				in >> task[i];
			}
			in >> token; assert(token == "]");

			// Get the array of minimum windows
			in >> token; assert(token == "minWindow");
			in >> token; assert(token == "=");
			in >> token; assert(token == "[");
			for (int i = 0; i < nLocations; i++) {
				in >> minWindow[i];
			}
			in >> token; assert(token == "]");

			// Get the array of maximum windows
			in >> token; assert(token == "maxWindow");
			in >> token; assert(token == "=");
			in >> token; assert(token == "[");
			for (int i = 0; i < nLocations; i++) {
				in >> maxWindow[i];
			}
			in >> token; assert(token == "]");

			in.close();
		}
};

#endif // INPUT_DATA_HPP
