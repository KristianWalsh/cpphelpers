#include "Events.hpp"





/////////////////////////////////////////////////////////////////////////////////////////////

Event::~Event()
{

}

 
/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////

EventDispatcher& EventDispatcher::instance( void )
{
  static EventDispatcher* instance = 0L;

  if (instance == 0L)
  {
    instance = new EventDispatcher;
  }

  return *instance; 
}

/////////////////////////////////////////////////////////////////////////////////////////////

EventDispatcher::EventDispatcher( )
{
  
}
  
/////////////////////////////////////////////////////////////////////////////////////////////

EventDispatcher::~EventDispatcher( )
{   
}

/////////////////////////////////////////////////////////////////////////////////////////////

void EventDispatcher::subscribe( EventType theEventID, EventConduit* responderConduit, void* sender )
{
  allResponders[sender][theEventID].push_back(responderConduit);
}

/////////////////////////////////////////////////////////////////////////////////////////////

void EventDispatcher::unsubscribe( EventType theEventID, EventConduit* responderObject, void* sender )
{
  EventRegistry::iterator p = allResponders.find(sender);
  if ( p!=allResponders.end())
  {
    EventRegistryForSender& senderListeners(p->second);
    EventRegistryForSender::iterator j = senderListeners.find(theEventID);
    if (j!=senderListeners.end())
    {
      ResponderListForEvent& responders( j->second ); 
      ResponderListForEvent::iterator i = responders.begin();
      while (i!=responders.end())
      {
        if ( *i == responderObject )
        {
          responders.erase( i );
          break;
        }
        ++i;
      }
    }
  }
}
  
/////////////////////////////////////////////////////////////////////////////////////////////

void EventDispatcher::unsubscribeAll( EventType theEventID, EventConduit* responderObject )
{
  for (EventRegistry::iterator i=allResponders.begin() ; i!=allResponders.end(); ++i) 
  {
    EventRegistryForSender::iterator j = i->second.find(theEventID);
    if (j!=i->second.end())
    {
      ResponderListForEvent& responders(j->second);
      for (ResponderListForEvent::iterator k=responders.begin(); k!=responders.end(); ++k)
      {
        if ( *k == responderObject )
        {
          Lock l(this);
          responders.erase( k );
          break;
        }
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////

void EventDispatcher::postEvent( const Event* eventObject, void* sender )
{
  { // this section is controlled by mutex in threaded environments...
    Lock l(this); // (mutex lasts for lifetime of the Lock object)
    eventQueue.push_back( EventQueueEntry( eventObject, sender ) );
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////

void EventDispatcher::postEventLater( const Event* eventObject, void* sender )
{
  { // this section is controlled by mutex in threaded environments...
    Lock l(this); // (mutex lasts for lifetime of the Lock object)
    deferredEventQueue.push_back( EventQueueEntry( eventObject, sender ) );
  }
}



/////////////////////////////////////////////////////////////////////////////////////////////

void EventDispatcher::processEvents_(void )
{
  unsigned int eventIndex = 0;

    // note - the size() check is performed on each iteration. this is correct, as
    // events can be added to the queue while we're running ProcessEvents.
  while (eventIndex < eventQueue.size())
  {
    const Event* eventObject = 0;
    void* sender = 0;
    { // Get event object from queue [this section is controlled by mutex in threaded environments]
      Lock l(this); // (mutex lasts for lifetime of the Lock object)
      eventObject = eventQueue[eventIndex].event;
      sender = eventQueue[eventIndex].sender;
    } //
    
    if ( eventObject )
    {
      EventRegistry::iterator place = allResponders.find(sender); // sender==0 means that this is a broadcast event
      if (place != allResponders.end())
      {
        EventRegistryForSender& respondersForSender( place->second );
        EventType eventType = &typeid(*eventObject);
        EventRegistryForSender::iterator j = respondersForSender.find(eventType);
        if (j!=respondersForSender.end())
        {
          ResponderListForEvent& responders( j->second ); 
          ResponderListForEvent::iterator i = responders.begin();
          while (i!=responders.end())
          {
            (*i)->callObjectEventHandler(eventObject);
            ++i;
          }
        }
      }

      { // Clear event object's place in queue [this section is controlled by mutex in threaded environments ]
        Lock l(this); // (mutex lasts for lifetime of the Lock object)
        eventQueue[eventIndex].event = 0L;
        delete eventObject;
      } //
    }
    
    ++eventIndex;
  }
  

  { // Clear the entire queue -- should be full of 0L slots by now [this section is controlled by mutex in threaded environments]
    Lock l(this); // (mutex lasts for lifetime of the Lock object)
    eventQueue=deferredEventQueue;
    deferredEventQueue.clear();
  } // 
}

