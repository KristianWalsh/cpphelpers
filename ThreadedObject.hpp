#ifndef ThreadedObject_hpp_1as234
#define ThreadedObject_hpp_1as234

#include <pthread.h>

class ThreadedObject
{
private:
  bool isRunning;
  bool quitRequested;

  pthread_t threadID;

  static void *threadRunner_( void* threadArg );

protected:
  bool shouldQuit( void ) {return quitRequested; };
  virtual void thread( )=0 ;
  
  void cancelThread() { if (isRunning) pthread_cancel(threadID); }
  
public:
  ThreadedObject( );
  virtual ~ThreadedObject( ); 
  void run( );
  bool running( ) { return isRunning; };
  virtual void requestQuit( ) { quitRequested = true; };
   
};

#endif
