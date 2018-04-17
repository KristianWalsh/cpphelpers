#ifndef _LOCKABLEOBJECT_H_
#define _LOCKABLEOBJECT_H_

/*
  LockableObject 
  --------------

    Thread-safe locking mix-in.   Objects that need a mutex for thread-safety can
    derive from LockableObject, and then create Lock objects as needed.

    Usage:


    // Critical section...
      { // create an empty scope: this controls lifetime of the Lock object.
        Lock l(this); // execution will not proceed until lock is granted.
          // Only one thread is accessing this object now, so
          // it's safe to change critical state information...

      } // object l is disposed of, and this mutex released, here
    // end of critical section

*/


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

#ifndef USE_THREADS__
// Bogus implementation for non-threaded applications
// (in this case, there's no need to include LockableObject.cpp in project)

class LockableObject 
{
};

class Lock
{
public:
  Lock( LockableObject* o ) {};
};

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#else

// Pthreads implementation

#include <pthread.h>

class LockableObject 
{
private:
  friend class Lock;
  pthread_mutex_t lock;
  
public:
  LockableObject();
  virtual ~LockableObject();
};

///////////////////////////////////////////////////////////////////////

/* Lock object. Create one of these to lock a LockableObject instance */
class Lock
{
private:
  LockableObject* lockedObject;
public:
  Lock( LockableObject* o );
  ~Lock( );
};
#endif

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

#endif
