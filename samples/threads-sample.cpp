#include "Events.hpp"
#include "Timer.hpp"
#include "ThreadedObject.hpp"
#include "LockableObject.hpp"
#include <unistd.h>
#include <iostream>

/*
  Sample code showing Event and ThreadedObject in use.
*/

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

  // declare ChangeDisplayedCharacterRequest containing a char, with value-getter 'char getCharacter();'
DECLARE_VALUE_EVENT(ChangeDisplayedCharacterRequest,char,getCharacter);

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

/*
  A SampleTask object represents the thread that prints a character to stdout once a second.
  If it receives a ChangeDisplayedCharacterRequest event, the character will
  be changed to whatever was sent as part of that event.
*/

class SampleTask : public ThreadedObject, public EventResponder<ChangeDisplayedCharacterRequest> 
{
  char character;

public:
  SampleTask(const char chr) : character(chr) { }
  virtual void respond(const ChangeDisplayedCharacterRequest&);

protected:
  virtual void thread( );
};

  // The actual thread that's run.
void SampleTask::thread()
{
  while (!shouldQuit())
  {
    printf("%c\n", character);
    usleep(1000000);
  }
}

  // Event responder for ChangeDisplayedCharacterRequest.  Changes the character displayed by
  // the thread to be the character in r.getCharacter()
void SampleTask::respond(const ChangeDisplayedCharacterRequest& r)
{
  character = r.getCharacter();
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

class Monitor : public EventResponder<ChangeDisplayedCharacterRequest>
{
  public:
    virtual void respond(const ChangeDisplayedCharacterRequest& r);

};

  // Event responder.  Changes the character displayed by
  // the thread to be the character in r.getCharacter()
void Monitor::respond(const ChangeDisplayedCharacterRequest& r)
{ 
    // right now, this only notes the event to stdout, but could take
    // any relevant action.
  std::cout<<"Request to change character to '"<<r.getCharacter()<<"' has been observed"<<std::endl;
}

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


int main(int argc, const char** argv)
{
  // create the char-printing thread.
  SampleTask t('*');
  Monitor m;  // try create two Monitors, see what happens:  Monitor m,m2; 

  Timer timeout;
  timeout.setDuration(5000000);  // 5 second timer
  timeout.restart();

    // start printing characters to stdout.
  t.run();
  char c='A';

    // The main loop. This doesn't exit.
  while (1)
  {
      // route any pending events to their destinations...
    EventDispatcher::process();

    if (timeout.expired())
    { 
        // broadcast a request to change the displayed character.
        // Only the SimpleTask thread responds to this event.
      EventDispatcher::broadcast(new ChangeDisplayedCharacterRequest(c));
      ++c;
      if (c>'Z') { c='A'; } 
      timeout.restart();  // timers don't auto-repeat
    }
    usleep(102034); // wait 102 ms to prevent busywaiting in main loop.
  }

}
