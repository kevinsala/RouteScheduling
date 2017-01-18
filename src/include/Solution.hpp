#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <list>
#include <ctime>
#include <limits>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Utils.hpp"
#include "Assignment.hpp"

const int EMPTY = -1;

class Solution {

	private:
		bool _feasible;
		Problem *_problem;

		float _objFunction;
		int _nCars;

		// Set of assigned locations
		std::list<int> _assigned;

		// Array of time arriving (indexed by id)
		std::vector<float> _arrive;

		// Matrix of prev and next (indexed by id)
		std::vector<int> _next;
		std::vector<int> _prev;

		// Array of start info (indexed by id)
		std::vector<bool> _startNext;
		std::vector<bool> _startPrev;
		std::vector<float> _startArrive;

	public:

		Solution( Problem &problem ) :
			_feasible( true ),
			_problem( &problem ),
			_objFunction( 0.0 ),
			_nCars( 0 ),
			_arrive( problem.getNLocations(), 0.0 ),
			_assigned(),
			_next( problem.getNLocations(), EMPTY ),
			_prev( problem.getNLocations(), EMPTY ),
			_startNext( problem.getNLocations(), false ),
			_startPrev( problem.getNLocations(), false ),
			_startArrive( problem.getNLocations(), 0.0 )
		{
			_assigned.push_back( problem.getStartLocation().getId() );
		}

		// Copy construction
        Solution( const Solution &other ) = default;

        // Copy assign operator
        Solution& operator=( const Solution &other ) = default;

		bool isFeasible()
		{
			return _feasible;
		}

		void makeInfeasible()
		{
			_feasible = false;
			_objFunction = std::numeric_limits<float>::infinity();
		}

		Problem &getProblem()
		{
			return *_problem;
		}

		float getObjFunction() const
		{
			return _objFunction;
		}

		void setObjFunction( float objFunction )
		{
			_objFunction = objFunction;
		}

		int getNCars()
		{
			return _nCars;
		}

		void setNCars( int nCars )
		{
			_nCars = nCars;
		}

		std::vector<float> &getArrive()
		{
			return _arrive;
		}

		int getPrevLocation( Location &location )
		{
			return _prev[location.getId()];
		}

		int getNextLocation( Location &location )
		{
			return _next[location.getId()];
		}

		std::list<Assignment> findFeasibleAssignments( int target, double &elapsedEvalTime, long long &evalCandidates )
		{
			std::clock_t startEvalTime = clock();
			evalCandidates = 0;

			std::list<Assignment> feasibleAssignments;
			for ( int prev : _assigned ) {
				for ( int next : _assigned ) {
					Assignment assignment( target, prev, next, 0 );
					bool feasible = assign( assignment );

					++evalCandidates;
					if ( !feasible ) continue;

					assignment.setObjFunction( _objFunction );
					feasibleAssignments.push_back( assignment );

					unassign( assignment );
				}
			}

			elapsedEvalTime = ((double) (clock() - startEvalTime)) / CLOCKS_PER_SEC;
			return feasibleAssignments;
		}

		Assignment findBestFeasibleAssignment( int target )
		{
			Assignment bestAssig;

			for ( int prev : _assigned ) {
				for ( int next : _assigned ) {
					Assignment assig( target, prev, next, 0 );
					bool feasible = assign( assig );
					if ( !feasible ) continue;

					if ( bestAssig.getObjFunction() > _objFunction ) {
						bestAssig = assig;
						bestAssig.setObjFunction( _objFunction );
					}

					unassign( assig );
				}
			}

			return bestAssig;
		}

		Assignment getAssignment( int location )
		{
			return Assignment( location, _prev[location], _next[location], 0.0 );
		}

		std::vector<Assignment> getLocationAssignments()
		{
			std::vector<Location> &locations = _problem->
				getTaskLocationsSortedByMinWindowDescending();
			std::vector<Assignment> assigs( locations.size() );

			for ( int i = 0; i < locations.size(); ++i )
				assigs[i] = getAssignment( locations[i].getId() );

			return assigs;
		}

		std::vector<Assignment> getLocationAssignmentsSortedByPathLength()
		{
			int nLocations = _problem->getNLocations();
			int start = _problem->getStartLocationId();
			std::vector< std::pair<int,float> > locs_and_paths;
			std::vector<Assignment> assigs( nLocations - 1 );

			for ( int i = 0; i < nLocations; ++i ) {
				if ( _startPrev[i] ) {
					float arrive = _startArrive[i];
					int location = i;
					assert( location != start);

					while ( location != start ) {
						assert(location != EMPTY);
						locs_and_paths.emplace_back( location, arrive );
						location = _prev[location];
					}
				}
			}

			assert(locs_and_paths.size() == nLocations-1);
			std::sort( locs_and_paths.begin(), locs_and_paths.end(), compareByPathLength );
			for ( int i = 0; i < nLocations - 1; ++i )
				assigs[i] = getAssignment( locs_and_paths[i].first );

			return assigs;
		}

		static bool compareByPathLength( const std::pair<int,float> &first, const std::pair<int,float> &second )
		{
			return first.second > second.second;
		}

		std::vector< std::pair<int,int> > getCurrentEdges()
		{
			int nLocations = _problem->getNLocations();
			int start = _problem->getStartLocationId();
			std::vector< std::pair<int,int> > assigs;

			for ( int i = 0; i < nLocations; ++i ) {
				assert( _next[i] != EMPTY );
				assigs.emplace_back( i, _next[i] );
			}

			for ( int i = 0; i < nLocations; ++i ) {
				if ( _startNext[i] != EMPTY )
					assigs.emplace_back( start, i );
			}

			return assigs;
		}

		bool assign( Assignment &assignment )
		{
			if ( !isFeasibleToAssignLocation( assignment ) )
				return false;

			Problem &problem = *_problem;
			int startId = problem.getStartLocationId();
			int targetId = assignment.getTargetLocation();
			int prevId = assignment.getPrevLocation();
			int nextId = assignment.getNextLocation();

			Location &start = problem.getLocation( startId );
			Location &target = problem.getLocation( targetId );
			Location &prev = problem.getLocation( prevId );
			Location &next = problem.getLocation( nextId );

			matrix<float> &dist = problem.getDistances();

			float arriveAtTarget = dist[prevId][targetId];
			if ( prevId != startId ) {
				arriveAtTarget += _arrive[prevId];
				arriveAtTarget += prev.getTaskDuration();
			}
			arriveAtTarget = std::max( arriveAtTarget, target.getMinWindow() );
			if ( arriveAtTarget > target.getMaxWindow() ) return false;

			std::vector<float> arriveAt;
			float lastArrive = arriveAtTarget;
			int lastId = targetId;
			int currId = nextId;
			bool end = false;

			while ( !end ) {
				assert(currId != EMPTY);
				Location &curr = problem.getLocation( currId );
				Location &last = problem.getLocation( lastId );

				// Compute the current arriving time
				float currArrive = lastArrive
					+ last.getTaskDuration()
					+ dist[lastId][currId];
				currArrive = std::max( currArrive, curr.getMinWindow() );

				// The assignment is infeasible if it not satifies
				// the maximum arriving time of the current location
				if ( currArrive > curr.getMaxWindow() ) return false;

				// Save the arriving time
				arriveAt.push_back( currArrive );
				lastArrive = currArrive;

				// Finish when the start location
				// has been processed
				end = (currId == startId);

				// Move to the next location
				lastId = currId;
				currId = _next[currId];
			}

			// At this point, we have checked that the assignment
			// is feasible, so we can update the state of the solution

			// Set the the arriving time at the target location
			_arrive[targetId] = arriveAtTarget;

			// Connect and disconnect the points depending on the case
			if ( prevId == startId && nextId == startId ) {
				_prev[targetId] = startId;
				_next[targetId] = startId;
				_startPrev[targetId] = true;
				_startNext[targetId] = true;
			}
			else if ( prevId == startId ) {
				_startNext[nextId] = false;
				_startNext[targetId] = true;
				_prev[nextId] = targetId;
				_prev[targetId] = startId;
				_next[targetId] = nextId;
			}
			else if ( nextId == startId ) {
				_startPrev[prevId] = false;
				_startPrev[targetId] = true;
				_next[prevId] = targetId;
				_prev[targetId] = prevId;
				_next[targetId] = startId;
			}
			else {
				_next[prevId] = targetId;
				_prev[nextId] = targetId;
				_prev[targetId] = prevId;
				_next[targetId] = nextId;
			}

			int i = 0;
			currId = nextId;
			lastId = targetId;

			while ( currId != startId ) {
				// Update the new arriving times
				_arrive[currId] = arriveAt[i];

				lastId = currId;
				currId = _next[currId];
				++i;
			}

			// Set the arriving time at the start location
			_startArrive[lastId] = arriveAt[i];

			// Update derived information
			updateArriveAtStart();
			updateNCars();
			updateObjFunction();

			// Add the location into the assigned locations
			_assigned.push_back( targetId );

			return true;
		}

		bool unassign( Assignment &assignment )
		{
			if ( !isFeasibleToUnassignLocation( assignment ) )
				return false;

			Problem &problem = *_problem;
			int startId = problem.getStartLocationId();
			int targetId = assignment.getTargetLocation();
			int prevId = _prev[targetId];
			int nextId = _next[targetId];
			assert(prevId != EMPTY);
			assert(nextId != EMPTY);

			Location &start = problem.getLocation( startId );
			Location &target = problem.getLocation( targetId );
			Location &prev = problem.getLocation( prevId );
			Location &next = problem.getLocation( nextId );

			if ( prevId == startId && nextId == startId ) {
				_startPrev[targetId] = false;
				_startNext[targetId] = false;
			}
			else if ( prevId == startId ) {
				_startNext[targetId] = false;
				_startNext[nextId] = true;
				_prev[nextId] = startId;
			}
			else if ( nextId == startId ) {
				_next[prevId] = startId;
				_startPrev[targetId] = false;
				_startPrev[prevId] = true;
			}
			else {
				_next[prevId] = nextId;
				_prev[nextId] = prevId;
			}

			// Disconnect the target location
			_prev[targetId] = EMPTY;
			_next[targetId] = EMPTY;
			_arrive[targetId] = 0.0;

			if ( prevId != startId || nextId != startId ) {
				matrix<float> &dist = problem.getDistances();
				float lastArrive = ( prevId != startId ) ? _arrive[prevId] : 0.0;
				int lastId = prevId;
				int currId = nextId;

				while ( currId != startId ) {
					Location &curr = problem.getLocation( currId );
					Location &last = problem.getLocation( lastId );

					// Compute the current arriving time
					float currArrive = lastArrive
						+ last.getTaskDuration()
						+ dist[lastId][currId];
					currArrive = std::max( currArrive, curr.getMinWindow() );
					assert(currArrive <= curr.getMaxWindow());

					// Update the arriving time
					_arrive[currId] = currArrive;
					lastArrive = currArrive;

					// Move to the next location
					lastId = currId;
					currId = _next[currId];
				}

				// Update the arriving time at the start location
				float arriveAtStart = lastArrive
					+ problem.getLocation( lastId ).getTaskDuration()
					+ dist[lastId][startId];
				assert(arriveAtStart <= 720);
				_startArrive[lastId] = arriveAtStart;
			}
			else _startArrive[targetId] = 0.0;

			// Update derived information
			updateArriveAtStart();
			updateNCars();
			updateObjFunction();

			// Remove the location from the assigned locations
			_assigned.remove( targetId );

			return true;
		}

		bool isFeasibleToAssignLocation( Assignment &assignment )
		{
			Problem &problem = *_problem;
			int startId = problem.getStartLocationId();
			int targetId = assignment.getTargetLocation();
			int prevId = assignment.getPrevLocation();
			int nextId = assignment.getNextLocation();

			if ( prevId == nextId && prevId != startId )
				return false;

			Location &target = problem.getLocation( targetId );
			Location &prev = problem.getLocation( prevId );
			Location &next = problem.getLocation( nextId );

			bool targetFound = false;
			bool prevFound = false;
			bool nextFound = false;

			for ( int location : _assigned ) {
				targetFound = targetFound || location == targetId;
				prevFound = prevFound || location == prevId;
				nextFound = nextFound || location == nextId;
			}

			// The assignation is not feasible if the target has been already
			// assigned or the previous and the next have not been assigned
			// before
			if ( targetFound || !prevFound || !nextFound )
				return false;

			// Check the connections of the paths
			if ( prevId == startId && nextId != startId ) {
				if ( _prev[nextId] != startId || !_startNext[nextId] )
					return false;
			}
			else if ( prevId != startId && nextId == startId ) {
				if ( _next[prevId] != startId || !_startPrev[prevId] )
					return false;
			}
			else if ( prevId != startId && nextId != startId )
				if ( _next[prevId] != nextId || _prev[nextId] != prevId )
					return false;

			return true;
		}

		bool isFeasibleToUnassignLocation( Assignment &assignment )
		{
			int target = assignment.getTargetLocation();
			int start = _problem->getStartLocation().getId();
			return target != start;
		}

		void updateNCars()
		{
			int nLocations = _problem->getNLocations();
			int outEdges = 0;
			int inEdges = 0;

			for (int i = 0; i < nLocations; ++i) {
				inEdges += _startPrev[i];
				outEdges += _startNext[i];
			}
			assert(inEdges == outEdges);

			_nCars = inEdges;
		}

		void updateArriveAtStart()
		{
			int startId = _problem->getStartLocationId();
			int nLocations = _problem->getNLocations();
			float maxArrive = 0.0;

			for (int i = 0; i < nLocations; ++i) {
				if ( _startPrev[i] && maxArrive < _startArrive[i] )
					maxArrive = _startArrive[i];
			}

			_arrive[startId] = maxArrive;
		}

		void updateObjFunction()
		{
			int startId = _problem->getStartLocationId();

			_objFunction = 720 * _nCars + _arrive[startId];
		}

		void saveToFile( std::string solutionFile )
		{
			std::ofstream out;
			out.open( solutionFile );

			if ( _feasible ) {
				out << "objFunction = " << _objFunction << std::endl;
				out << "nCars = " << _nCars << std::endl;

				int nLocations = _problem->getNLocations();
				int start = _problem->getStartLocationId();
				matrix<bool> travel( nLocations, std::vector<bool>( nLocations, false ) );
				for (int i = 0; i < nLocations; ++i) {
					if ( i != start )
						travel[i][_next[i]] = true;
					travel[start][i] = _startNext[i];
				}

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
					out << _arrive[i] << " ";
				out << "]" << std::endl;
			}
			else out << "This instance is infeasible" << std::endl;

			out.close();
		}
};

#endif // SOLUTION_HPP
