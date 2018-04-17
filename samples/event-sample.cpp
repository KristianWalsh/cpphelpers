#include <iostream>
#include <string>

#include "Events.hpp"

int mooEventInstances;

class MooEvent : public Event
{
public:
  MooEvent() { ++mooEventInstances; }
  virtual ~MooEvent() { --mooEventInstances; }
};

    // this is a "plain old" event; no data, no special methods 
DECLARE_EVENT( BaaEvent )


    // declare a responder for all events
class BroadcastReceiver
  : public EventResponder<MooEvent>,
    public EventResponder<BaaEvent>
{
  public:
    BroadcastReceiver() : EventResponder<MooEvent>(), receiveCountMoo(0), receiveCountBaa(0) { }
    int receiveCountMoo;
    int receiveCountBaa;

    virtual void respond( const MooEvent& e);
    virtual void respond( const BaaEvent& e);
};

void BroadcastReceiver::respond( const MooEvent& e) 
{
  printf("Broadcast listener sees MooEvent!\n");
  ++receiveCountMoo;
}

void BroadcastReceiver::respond( const BaaEvent& e) 
{ 
    printf("Broadcast listener sees BaaEvent!\n");
    ++receiveCountBaa;
}



//---------------------------------------------------------

  // SelectiveReceiver doesn't respond to anything until it subscribes
class SelectiveReceiver
  : public EventResponder<MooEvent>
{
  public:
    SelectiveReceiver();
    int receiveCount;

    virtual void respond( const MooEvent& e);
};


SelectiveReceiver::SelectiveReceiver() 
  : EventResponder<MooEvent>( EventDispatcher::NoBroadcasts ),
    receiveCount(0) 
  {

  }

void SelectiveReceiver::respond( const MooEvent& e) 
{
  ++receiveCount;
}

//--------------------------------------------------------

  // Convenience object, to avoid repeating EventDispatcher::send( ) in the code
class Sender
{
public:
  void sendEvent( ) { EventDispatcher::send( new MooEvent, this ); }
  void broadcastEvent( ) { EventDispatcher::broadcast( new MooEvent ); }
 
  void broadcastBaaEvent( ) { EventDispatcher::broadcast( new BaaEvent ); }
  void broadcastBaaEventLater() { EventDispatcher::broadcastLater( new BaaEvent ); }
};

// -------------------------------------------

/*  Reposter receives MooEvent objects, but broadcasts BaaEvent objects in response.
    Here, it will be used to illustrate the difference between postLater and post().
*/

class Reposter
  : public EventResponder<MooEvent>
{
  public:
    Reposter( Sender& s) : EventResponder<MooEvent>(), sender(&s), receiveCount(0), repostLater(false) { }
    Sender* sender;
    int receiveCount;
    bool repostLater;

    virtual void respond( const MooEvent& e) {++receiveCount;  if (repostLater) {sender->broadcastBaaEventLater();} else {sender->broadcastBaaEvent();} }
};

///////////////////////////////////////////////////////////////////////

int main(int argc, const char** argv)
{
  mooEventInstances = 0;
  BroadcastReceiver br;
  SelectiveReceiver sr;
  Sender s;

  s.broadcastEvent(); // post a MooEvent to the event queue.
  EventDispatcher::process();

  std::cout<<" Broadcast listener: MooEvent Received = "<< br.receiveCountMoo<<std::endl;  // broadcast listener heard event
  std::cout<<" Selective listener: MooEvent Received = "<< sr.receiveCount<<std::endl;  // broadcast listener heard event

  std::cout<<" Send selectively (there are no subscribers yet"<<std::endl;
  s.sendEvent();  // nothing has subscribed yet.
  EventDispatcher::process();
  std::cout<<" Broadcast listener: MooEvent Received = "<< br.receiveCountMoo<<std::endl;  // broadcast listener heard event
  std::cout<<" Selective listener: MooEvent Received = "<< sr.receiveCount<<std::endl;  // broadcast listener heard event

  std::cout<<" Subscribe "<<std::endl;
  EVENT_SUBSCRIBE( sr, MooEvent, &s);
  std::cout<<" Send selectively (one subscriber)"<<std::endl;
  s.sendEvent();  // subscribed listener should see it.
  EventDispatcher::process();
  std::cout<<" Broadcast listener: MooEvent Received = "<< br.receiveCountMoo<<std::endl;  // broadcast listener heard event
  std::cout<<" Selective listener: MooEvent Received = "<< sr.receiveCount<<std::endl;  // broadcast listener heard event

  std::cout<<" Unsubscribe "<<std::endl;
  EVENT_UNSUBSCRIBE( sr, MooEvent, &s);
  std::cout<<" Send selectively (there are no subscribers any more)"<<std::endl;
  s.sendEvent();  // no subscribers again; nobody should see it.
  EventDispatcher::process();
  std::cout<<" Broadcast listener: MooEvent Received = "<< br.receiveCountMoo<<std::endl;  // broadcast listener heard event
  std::cout<<" Selective listener: MooEvent Received = "<< sr.receiveCount<<std::endl;  // broadcast listener heard event


  Reposter rp( s );
  std::cout<<" Subscribe re-poster to sender"<<std::endl;
  EVENT_SUBSCRIBE( rp, MooEvent, &s);
  std::cout<<" Send selectively (re-poster is subscribed)"<<std::endl;
  s.sendEvent();  
  EventDispatcher::process();
  std::cout<<" Re-posting listener: MooEvent Received = "<< rp.receiveCount<<std::endl;  // broadcast listener heard event
  std::cout<<" Broadcast listener: BaaEvent Received = "<< br.receiveCountBaa<<std::endl;  // broadcast listener heard event

  std::cout<<" Tell re-poster to use postLater()"<<std::endl;
  rp.repostLater=true;  // tell reposter to defer 
  std::cout<<" Send selectively (re-poster is subscribed)"<<std::endl;
  s.sendEvent();  // no subscribers again; nobody should see it.
  std::cout<<" Process events (will process the original event)"<<std::endl;
  EventDispatcher::process();
  std::cout<<" Re-posting listener: MooEvent Received = "<< rp.receiveCount<<std::endl;  // broadcast listener heard event
  std::cout<<" Broadcast listener: BaaEvent Received = "<< br.receiveCountBaa<<std::endl;  // broadcast listener heard event

  std::cout<<" Process events (will process the re-posted event)"<<std::endl;
  EventDispatcher::process(); // but once we ask to process events again...
  std::cout<<" Re-posting listener: MooEvent Received = "<< rp.receiveCount<<std::endl;  // broadcast listener heard event
  std::cout<<" Broadcast listener: BaaEvent Received = "<< br.receiveCountBaa<<std::endl;  // broadcast listener heard event


    return 0;   
}