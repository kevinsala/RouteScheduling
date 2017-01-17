#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>

#define MAXCOORD 250

int main( int argc, char **argv )
{
	if ( argc != 2 ) {
		std::cout << "./InstanceGenerator nLocations" << std::endl;
		exit( 1 );
	}

	int nLocations = atoi( argv[1] );
	int start = 0;

	std::vector< std::pair<int,int> > points( nLocations );
	std::vector<float> task( nLocations );
	std::vector<float> minWindow( nLocations );
	std::vector<float> maxWindow( nLocations );

	points[0] = std::pair<int,int>( MAXCOORD/2, MAXCOORD/2 );
	task[0] = 0.0;
	minWindow[0] = 0.0;
	maxWindow[0] = 720.0;

	for ( int i = 1; i < nLocations; ++i ) {
		int x = rand() % (MAXCOORD + 1);
		int y = rand() % (MAXCOORD + 1);
		points[i] = std::pair<int,int>( x, y );
		task[i] = 20 + rand() / (RAND_MAX/(60 - 20));
		minWindow[i] = 100 + rand() / (RAND_MAX/(300 - 100));
		maxWindow[i] = 400 + rand() / (RAND_MAX/(650 - 400));
	}

	std::ostringstream s;
	s << "input-" << nLocations << "locations.dat";
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
