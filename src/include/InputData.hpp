#ifndef INPUT_DATA_HPP
#define INPUT_DATA_HPP

#include <cmath>
#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include <iostream>

#include "Utils.hpp"

class InputData {

	public:

		int nLocations;
		int startLocation;

		matrix<float> distance;
		std::vector<float> task;
		std::vector<float> minWindow;
		std::vector<float> maxWindow;

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
			distance.resize( nLocations );
			for (int i = 0; i < nLocations; ++i)
				distance[i].resize( nLocations );

			task.resize( nLocations );
			minWindow.resize( nLocations );
			maxWindow.resize( nLocations );
			matrix<int> p(nLocations, std::vector<int>(2, 0));

			in >> token;
			int fromPoints = 0;
			if ( token == "fromPoints" ) {
				in >> token; assert(token == "=");
				in >> fromPoints;

				in >> token; assert(token == "points");
				in >> token; assert(token == "=");
				in >> token; assert(token == "[");
				for (int i = 0; i < nLocations; i++) {
					in >> token; assert(token == "[");
					in >> p[i][0] >> p[i][1];
					in >> token; assert(token == "]");
				}
				in >> token; assert(token == "]");
				in >> token;
			}

			// Get the matrix of distances
			assert(token == "distance");
			in >> token; assert(token == "=");
			in >> token; assert(token == "[");
			for (int i = 0; i < nLocations; i++) {
				in >> token; assert(token == "[");
				for (int j = 0; j < nLocations; j++) {
					in >> distance[i][j];
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

			if ( fromPoints ) {
				for (int i = 0; i < nLocations; ++i) {
					for (int j = 0; j < nLocations; ++j) {
						distance[i][j] = sqrt(pow(p[i][0]-p[j][0],2) + pow(p[i][1]-p[j][1],2));
					}
				}
			}

			in.close();
		}
};

#endif // INPUT_DATA_HPP
