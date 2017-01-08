#ifndef PROBLEM_HPP
#define PROBLEM_HPP

#include <algorithm>
#include <vector>
#include <list>

#include "InputData.hpp"
#include "Location.hpp"
#include "Utils.hpp"

class Problem {

	private:
		int _nLocations;
		std::vector<Location> _locations;
		matrix<float> _distances;

		Location _startLocation;
		std::list<Location> _taskLocations;

	public:
		Problem( InputData &input ) :
			_nLocations( input.nLocations ),
			_locations(),
			_distances( input.distance ),
			_startLocation( input.startLocation, 0, 0, 720 ),
			_taskLocations()
		{
			int start = input.startLocation;
			std::vector<float> &task = input.task;
			std::vector<float> &minWin = input.minWindow;
			std::vector<float> &maxWin = input.maxWindow;

			// Initialize the locations
			for (int l = 0; l < _nLocations; ++l) {
				_locations.emplace_back( l, task[l], minWin[l], maxWin[l] );
				if ( l != start )
					_taskLocations.emplace_back( l, task[l], minWin[l], maxWin[l] );
			}
		}

		bool checkFeasibility()
		{
			for ( Location &location : _taskLocations ) {
				float dist = getDistance( _startLocation, location );
				if ( dist > location.getMaxWindow() ) return false;
				dist = std::max( dist, location.getMinWindow() );
				dist += location.getTaskDuration();
				dist += getDistance( location, _startLocation );
				if ( dist > 720 ) return false;
			}
			return true;
		}

		int getNLocations()
		{
			return _nLocations;
		}

		std::vector<Location> &getLocations()
		{
			return _locations;
		}

		Location &getLocation( int id )
		{
			return _locations[id];
		}

		int getStartLocationId()
		{
			return _startLocation.getId();
		}

		Location &getStartLocation()
		{
			return _startLocation;
		}

		std::list<Location> getTaskLocationList()
		{
			return _taskLocations;
		}

		matrix<float> &getDistances()
		{
			return _distances;
		}

		float getDistance( Location &l1, Location &l2 )
		{
			return _distances[l1.getId()][l2.getId()];
		}

		float getDistance( int id1, int id2 )
		{
			return _distances[id1][id2];
		}
};

#endif // PROBLEM_HPP
