#ifndef __TIMER_H_
#define __TIMER_H_

#include <inttypes.h>
#include <sys/time.h>



	/*!
		Lightweight timer class.
	 
		Timers require no system resources while "running".
	 */
class Timer
{
private:
	struct timeval lastReset;
	uint32_t delayInMicroseconds;
	bool live;
	
public:
	/*! @function  Create timer, set it running */
	Timer( uint32_t durationMicroseconds =0L );
	bool expired( void ) const ;	///< Report if timer has triggered.  Non-running timers never expire.
	bool running( void ) const { return (live && !expired()); }	///< Report if timer is running. Note that this is not the same as !expired()
	void restart(void);	///< Reset timer. Resets timer and starts it running.
	void stop( void ); ///< Stop timer. All subsequent calls to HasTimedOut() will return false (a stopped timer cannot expire) 
	void setDuration( uint32_t newDelayMicroseconds ); ///< Change timer delay. Timer will still continue to "run" if running.
	uint32_t duration( void ) const {return delayInMicroseconds; };	///< Report delay
};


#endif
