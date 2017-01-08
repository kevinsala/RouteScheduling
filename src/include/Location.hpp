#ifndef LOCATION_HPP
#define LOCATION_HPP

class Location {

	private:
		int _id;
		float _taskDuration;
		float _minWindow;
		float _maxWindow;

	public:
		Location() :
			_id( 0 ),
			_taskDuration( 0.0 ),
			_minWindow( 0.0 ),
			_maxWindow( 0.0 )
		{
		}

		Location( int id ) :
			_id( id ),
			_taskDuration( 0.0 ),
			_minWindow( 0.0 ),
			_maxWindow( 0.0 )
		{
		}

		Location( int id, float taskDuration, int minWindow, int maxWindow ) :
			_id( id ),
			_taskDuration( taskDuration ),
			_minWindow( minWindow ),
			_maxWindow( maxWindow )
		{
		}

		int getId()
		{
			return _id;
		}

		void setId( int id )
		{
			_id = id;
		}

		float getTaskDuration()
		{
			return _taskDuration;
		}

		void setTaskDuration( float taskDuration )
		{
			_taskDuration = taskDuration;
		}

		float getMinWindow()
		{
			return _minWindow;
		}

		void setMinWindow( float minWindow )
		{
			_minWindow = minWindow;
		}

		float getMaxWindow()
		{
			return _maxWindow;
		}

		void setMaxWindow( float maxWindow )
		{
			_maxWindow = maxWindow;
		}

		static bool compareByMinWindowDescending( const Location& first, const Location& second )
		{
			return first._minWindow > second._minWindow;
		}
};

#endif // LOCATION_HPP
