#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <ctime>
#include <limits>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Utils.hpp"
#include "Assignment.hpp"

class Solution {

	private:
		bool _feasible;
		Problem *_problem;

		float _objFunction;
		int _nCars;

		// Set of assigned locations
		std::vector<int> _assigned;

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
			_feasible( false ),
			_problem( &problem ),
			_objFunction( 0.0 ),
			_nCars( 0 ),
			_arrive( problem.getNLocations(), 0.0 ),
			_assigned(),
			_next( problem.getNLocations(), -1 ),
			_prev( problem.getNLocations(), -1 ),
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

		float getObjFunction()
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

		std::list<Assignment> findFeasibleAssignments( int target, float &elapsedEvalTime, int &evalCandidates )
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

			elapsedEvalTime = ((float) (clock() - startEvalTime)) / CLOCKS_PER_SEC;
			return feasibleAssignments;
		}

		// TODO: Solution::assign
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
				assert(currId != -1);
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
				_startArrive[prevId] = 0.0;
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

			return true;
		}

		// TODO: Solution::unassign
		bool unassign( Assignment &assignment )
		{
			if ( !isFeasibleToUnassignLocation( assignment ) )
				return false;
		}

		bool isFeasibleToAssignLocation( Assignment &assignment )
		{
			Problem &problem = *_problem;
			int startId = problem.getStartLocationId();
			int targetId = assignment.getTargetLocation();
			int prevId = assignment.getPrevLocation();
			int nextId = assignment.getNextLocation();

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
				if ( _next[prevId] == nextId || _prev[nextId] == prevId )
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

		// TODO: Solution::saveToFile
		void saveToFile( std::string solutionFile )
		{
			std::ofstream out;
			out.open( solutionFile );

			if ( _feasible ) {
				out << "objFunction = " << _objFunction << std::endl;
				out << "nCars = " << _nCars << std::endl;

				int nLocations = _problem->getNLocations();
				/*out << "travel = [" << std::endl;
				for ( int i = 0; i < nLocations; ++i ) {
					out << "[ ";
					for ( int j = 0; j < nLocations; ++j ) {
						out << _travel[i][j] << " ";
					}
					out << "]" << std::endl;
				}
				out << "]" << std::endl;*/

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
