#ifndef ASSIGNMENT_HPP
#define ASSIGNMENT_HPP

class Assignment {

	private:
		int _targetLocation;
		int _prevLocation;
		int _nextLocation;
		float _objFunction;

	public:
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
