#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <time.h>
#include <vector>
#include <algorithm>
#include <cmath>

#define MAXCOORD 250

int main( int argc, char **argv )
{
	if ( argc != 2 ) {
		std::cout << "./instancegenerator nLocations" << std::endl;
		exit( 1 );
	}

	srand( time( NULL ) );

	int nLocations = atoi( argv[1] );
	int start = 0;

	std::vector< std::pair<int,int> > points( nLocations );
	std::vector<float> task( nLocations );
	std::vector<float> minWindow( nLocations );
	std::vector<float> maxWindow( nLocations );
	std::vector< std::vector<float> > dist( nLocations, std::vector<float> ( nLocations, 0 ) );

	points[0] = std::pair<int,int>( MAXCOORD/2, MAXCOORD/2 );
	task[0] = 0.0;
	minWindow[0] = 0.0;
	maxWindow[0] = 720.0;

	for ( int i = 1; i < nLocations; ++i ) {
		int x = rand() % (MAXCOORD + 1);
		int y = rand() % (MAXCOORD + 1);
		points[i] = std::pair<int,int>( x, y );
		task[i] = 20 + rand() / (RAND_MAX/(60 - 20));
		minWindow[i] = 150 + rand() / (RAND_MAX/(230 - 150));
		maxWindow[i] = 470 + rand() / (RAND_MAX/(610 - 470));
	}

	for (int i = 0; i < nLocations; ++i) {
		for (int j = 0; j < nLocations; ++j) {
			dist[i][j] = sqrt(pow(points[i].first-points[j].first,2) + pow(points[i].second-points[j].second,2));
		}
	}

	bool feasible = true;
	for ( int i = 1; i < nLocations; ++i ) {
    	float mydist = dist[start][i];
		if ( mydist > maxWindow[i] ) feasible = false;
		mydist = std::max( mydist, minWindow[i] );
		mydist += task[i];
		mydist += dist[i][start];
		if ( mydist > 720 ) feasible = false;
	}

	if ( !feasible ) {
		std::cout << "Instance not feasible! Repeat please" << std::endl;
		exit( 1 );
	}

	// Metaheuristics input file
	{
		std::ostringstream s;
		s << "instances/input" << nLocations << "locs.mh.dat";
		std::string filename( s.str() );
	
		std::ofstream file;
		file.open( filename.c_str() );
	
		file << "nLocations = " << nLocations << std::endl;
		file << "startLocation = " << start << std::endl;
		file << "fromPoints = " << 1 << std::endl;
	
		file << "points = [" << std::endl;
		for ( int i = 0; i < nLocations; ++i ) {
			file << "[ " << points[i].first << " " << points[i].second << " ]" << std::endl;
		}
		file << "]" << std::endl;
	
		file << "distance = [" << std::endl;
		for ( int i = 0; i < nLocations; ++i ) {
			file << "[ ";
			for ( int j = 0; j < nLocations; ++j ) {
				file << 0 << " ";
			}
			file << "]" << std::endl;
		}
		file << "]" << std::endl;
	
		file << "task = [ ";
		for ( int i = 0; i < nLocations; ++i ) {
			file << task[i] << " ";
		}
		file << "]" << std::endl;
	
		file << "minWindow = [ ";
		for ( int i = 0; i < nLocations; ++i ) {
			file << minWindow[i] << " ";
		}
		file << "]" << std::endl;
	
		file << "maxWindow = [ ";
		for ( int i = 0; i < nLocations; ++i ) {
			file << maxWindow[i] << " ";
		}
		file << "]" << std::endl;

		file.close();
	}

	// CPLEX input file
	{
		std::ostringstream s;
		s << "instances/input" << nLocations << "locs.cplex.dat";
		std::string filename( s.str() );

		std::ofstream file;
		file.open( filename.c_str() );
	
		file << "nLocations = " << nLocations << ";" << std::endl;
		file << "ls = " << (start + 1) << ";" << std::endl;
		file << "fromPoints = " << 1 << ";" << std::endl;
	
		file << "points = [" << std::endl;
		for ( int i = 0; i < nLocations; ++i ) {
			file << "[ " << points[i].first << " " << points[i].second << " ]" << std::endl;
		}
		file << "];" << std::endl;
	
		file << "dist = [" << std::endl;
		for ( int i = 0; i < nLocations; ++i ) {
			file << "[ ";
			for ( int j = 0; j < nLocations; ++j ) {
				file << 0 << " ";
			}
			file << "]" << std::endl;
		}
		file << "];" << std::endl;
	
		file << "t = [ ";
		for ( int i = 0; i < nLocations; ++i ) {
			file << task[i] << " ";
		}
		file << "];" << std::endl;
	
		file << "minWin = [ ";
		for ( int i = 0; i < nLocations; ++i ) {
			file << minWindow[i] << " ";
		}
		file << "];" << std::endl;
	
		file << "maxWin = [ ";
		for ( int i = 0; i < nLocations; ++i ) {
			file << maxWindow[i] << " ";
		}
		file << "];" << std::endl;

		file.close();
	}
}
