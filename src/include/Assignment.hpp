#ifndef ASSIGNMENT_HPP
#define ASSIGNMENT_HPP

#include <limits>

class Assignment {

	private:
		int _targetLocation;
		int _prevLocation;
		int _nextLocation;
		float _objFunction;

	public:
		Assignment() :
			_targetLocation( -1 ),
			_prevLocation( -1 ),
			_nextLocation( -1 ),
			_objFunction( std::numeric_limits<float>::infinity() )
		{
		}

		Assignment( int targetLocation, int prevLocation, int nextLocation, float objFunction ) :
			_targetLocation( targetLocation ),
			_prevLocation( prevLocation ),
			_nextLocation( nextLocation ),
			_objFunction( objFunction )
		{
		}

		int getTargetLocation()
		{
			return _targetLocation;
		}

		int getPrevLocation()
		{
			return _prevLocation;
		}

		int getNextLocation()
		{
			return _nextLocation;
		}

		float getObjFunction()
		{
			return _objFunction;
		}

		void setObjFunction( float objFunction )
		{
			_objFunction = objFunction;
		}

		static bool compareByObjFunctionAscending( const Assignment &first, const Assignment &second )
		{
			return first._objFunction < second._objFunction;
		}
};

#endif // ASSIGNMENT_HPP
