
#include "ThreadedObject.hpp"
#include <signal.h>

////////////////////////////////////////////////////////////

void ThreadedObject::run( )
{
  if (!isRunning)
  {
    pthread_create(&threadID, 0L, ThreadedObject::threadRunner_, this);
  }
}

////////////////////////////////////////////////////////////

void *ThreadedObject::threadRunner_( void* threadArg )
{
  ThreadedObject* instance = reinterpret_cast<ThreadedObject*>( threadArg );
  instance->isRunning = true;
  instance->quitRequested = false;
  instance->thread(); 
  instance->isRunning = false;
  return 0L;
}

////////////////////////////////////////////////////////////

ThreadedObject::ThreadedObject()
: isRunning(false), quitRequested(false)
{
  
}

////////////////////////////////////////////////////////////

ThreadedObject::~ThreadedObject( )
{
  if (isRunning )
  {
    pthread_kill(threadID, SIGKILL );
  }
}

////////////////////////////////////////////////////////////
