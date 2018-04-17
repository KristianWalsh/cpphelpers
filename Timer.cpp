#include "Timer.hpp"
#include <iostream>

using namespace std;

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

Timer::Timer( uint32_t delay )
{
	setDuration( delay );
	restart();
	live=false;
}

//////////////////////////////////////////////////////////////////////////////

bool Timer::expired( void ) const
{
	if (live)
	{
		struct timeval now;
		struct timezone dummy;
		gettimeofday( &now, &dummy );
		
		struct timeval elapseTime = lastReset;
		
		elapseTime.tv_usec += delayInMicroseconds%1000000;
		if (elapseTime.tv_usec > 1000000)
		{
			elapseTime.tv_sec++;
		}
		elapseTime.tv_sec += delayInMicroseconds/1000000;
		return (elapseTime.tv_sec< now.tv_sec || ( elapseTime.tv_sec == now.tv_sec && elapseTime.tv_usec < now.tv_usec ) );
	}
	else
	{
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////

void Timer::restart( void )
{
	struct timezone dummy;
	gettimeofday(&lastReset, &dummy);
	live=true;
}

//////////////////////////////////////////////////////////////////////////////


void Timer::stop( void )
{
	live=false;
}

//////////////////////////////////////////////////////////////////////////////

void Timer::setDuration( uint32_t delay )
{
	delayInMicroseconds = delay;
}

